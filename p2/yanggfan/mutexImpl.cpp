#include "mutex.h"
#include "mutexImpl.h"
#include "cpu.h"
#include "globals.h"
#include <stdexcept>

mutex::impl::impl() : locked{false} {

};

mutex::impl::~impl() {

};

void mutex::impl::lock() {
    assert_interrupts_disabled();

    // if not locked, lock and set owner as current thread
    if(!locked) {
        owner_ID = running_context.ID;                
        locked = true;
    }
    // else add thread to waiting queue and switch to next ready thread
    else {
        // if there are no ready threads, suspend cpu.
        if(readyQ.empty()){
            cpu::interrupt_enable_suspend();
        }
        else {
            mutexQ.push(running_context);
            switch_to_next_ready_thread();
        }
    }

    assert_interrupts_disabled();
};

void mutex::impl::unlock() {
    assert_interrupts_disabled();

    // check for misuses of mutexes
    if (!locked)
        throw std::runtime_error("a runtime error");
    else if(running_context.ID != owner_ID)
        throw std::runtime_error("a runtime error");
    
    // unlock and give lock to next waiting thread
    locked = false;
    if (!mutexQ.empty()) {
        readyQ.push(mutexQ.front());
        owner_ID = mutexQ.front().ID;
        mutexQ.pop();
        locked = true;
    }

    assert_interrupts_disabled();

};

