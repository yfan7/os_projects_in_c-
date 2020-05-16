#include "thread.h"
#include "threadImpl.h"
#include "globals.h"
#include <ucontext.h>
#include <stdexcept>
#include <queue>
#include <stdexcept>

thread::impl::impl(thread_startfunc_t func, void * arg) {
    assert_interrupts_disabled();

    // allocate stack for new thread
    char *stack = new char [STACK_SIZE];
    
    ucontext_t* ucontext_ptr = new ucontext_t;
    ucontext_ptr->uc_stack.ss_sp = stack;
    ucontext_ptr->uc_stack.ss_size = STACK_SIZE;
    ucontext_ptr->uc_stack.ss_flags = 0;
    ucontext_ptr->uc_link = nullptr;

    makecontext(ucontext_ptr, (void (*)()) starter, 4, (void *)func, (void *)arg, this);
    
    // add new thread to ready queue
    readyQ.push(context_with_ID{ucontext_ptr, new_ID});
    finished_threads.push_back(false);
    thread_ID = new_ID;
    new_ID++;


    assert_interrupts_disabled();
};

thread::impl::~impl() {
    
}

void thread::impl::starter(thread_startfunc_t func, void * arg, thread::impl* thread_class_ptr) {
    
    // delete allocated stack of thread that just finished running
    delete[] old_stack;
    old_stack = nullptr;

    assert_interrupts_disabled();
    cpu::interrupt_enable();
    func(arg);
    // func finished running
    assert_interrupts_enabled();
    cpu::interrupt_disable();

    finished_threads[running_context.ID] = true;
    // wakeup all threads that are waiting on this thread    
    if (joinQ.find(running_context.ID) != joinQ.end()){
        while(!joinQ[running_context.ID]->empty()) {            
            readyQ.push(joinQ[running_context.ID]->front());
            joinQ[running_context.ID]->pop();
        }
        delete joinQ[running_context.ID];
    }   
    
    // if there are no ready threads, suspend cpu.
    if (readyQ.empty()) {
        cpu::interrupt_enable_suspend();
    }
    else {
        ucontext_t* old_context = running_context.context_ptr;
        running_context = readyQ.front();
        readyQ.pop();
        old_stack = (char*) old_context->uc_stack.ss_sp;
        delete old_context;
        setcontext(running_context.context_ptr);
    }
    assert_interrupts_disabled();
    cpu::interrupt_enable();
}

void thread::impl::join() {
    assert_interrupts_disabled();
    
    // create a new joinQ pointer only when the thread is 
    // is called the join function
    if (joinQ.find(thread_ID) == joinQ.end()) {
        joinQ[thread_ID] = new std::queue<context_with_ID>();
    }                   
    
    if(!finished_threads[thread_ID]) {
        // if there are no ready threads, suspend cpu.
        if(readyQ.empty()){
            cpu::interrupt_enable_suspend();
        }
        else {
            joinQ[thread_ID]->push(running_context);
            switch_to_next_ready_thread();
        }
    }
    assert_interrupts_disabled();
};

