#include "thread.h"
#include "threadImpl.h"
#include <ucontext.h>
#include "globals.h"
#include <new>

thread::thread(thread_startfunc_t func, void * val) {
    interrupt_handler handler;

    try {
        impl_ptr = new impl(func, val);
    }
    catch (std::bad_alloc& e) {
        throw e;
    }

}

thread::~thread() {
    interrupt_handler handler;

    delete impl_ptr;
}

void thread::join() {
    interrupt_handler handler;
    
    impl_ptr->join();
}

void thread::yield() {
    interrupt_handler handler;

    if(!readyQ.empty()) { 
        readyQ.push(running_context);
        switch_to_next_ready_thread();
    }
}