#include "globals.h"
#include <ucontext.h>
#include <vector>
#include <unordered_map>

std::queue<context_with_ID> readyQ;

std::vector<bool> finished_threads;

context_with_ID running_context;

int new_ID = 0;

char* old_stack = nullptr;

std::unordered_map<int, std::queue<context_with_ID>*> joinQ;

//switch to the next context in the readyQ and clean up memory of the previous context
void switch_to_next_ready_thread() {
    ucontext_t* old_context = running_context.context_ptr;        
    running_context = readyQ.front();
    readyQ.pop();
    swapcontext(old_context, running_context.context_ptr);
    delete[] old_stack;
    old_stack = nullptr;
}


interrupt_handler::interrupt_handler() {
    assert_interrupts_enabled();
    cpu::interrupt_disable();
}

interrupt_handler::~interrupt_handler() {
    assert_interrupts_disabled();
    cpu::interrupt_enable();
}
