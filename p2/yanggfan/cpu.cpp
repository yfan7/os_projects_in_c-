#include "cpu.h"
#include "cpuImpl.h"
#include <ucontext.h>
#include "globals.h"

// function that will run when the CPU recieves a timer interrupt.
// switch to the next ready thread if one exists
void timer_interrupt() {
    interrupt_handler handler;
    if(!readyQ.empty()) {
        readyQ.push(running_context);
        switch_to_next_ready_thread();
   }
};

//do not need to implement
void ipi_interrupt() {};

void cpu::init(thread_startfunc_t func, void * arg){
    interrupt_vector_table[TIMER] = timer_interrupt;
    interrupt_vector_table[IPI] = ipi_interrupt;

    impl_ptr = new impl(func, arg);
};
