#include "cpu.h"
#include "cpuImpl.h"
#include <ucontext.h>
#include "globals.h"
#include "threadImpl.h"
#include <assert.h>

// Creates the first thread to call the function 
// pointed to by func with the argument arg
cpu::impl::impl(thread_startfunc_t func, void * arg) {
    assert_interrupts_disabled();
    if (func != nullptr) {
        thread::impl(func, arg);
    }
    assert(readyQ.size() != 0);
    running_context = readyQ.front();
    readyQ.pop();

    assert_interrupts_disabled();
    setcontext(running_context.context_ptr);

};