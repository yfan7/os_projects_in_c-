#include "cv.h"
#include "cvImpl.h"
#include "cpu.h"
#include "mutexImpl.h"

cv::impl::impl() {

};

cv::impl::~impl() {
    
};

void cv::impl::wait(mutex& m) {
    assert_interrupts_disabled();

    // unlock the mutex
    m.impl_ptr->unlock();
    
    // switch to next ready thread if one exists, else wait
    if (readyQ.empty()) {
        cpu::interrupt_enable_suspend();
    }
    else {
        waitingQ.push(running_context);
        switch_to_next_ready_thread();

        // lock the mutex
        m.impl_ptr->lock();
    }

    assert_interrupts_disabled();
};

void cv::impl::signal() {
    assert_interrupts_disabled();

    // wake up one thread
    if(!waitingQ.empty()) {
        readyQ.push(waitingQ.front());
        waitingQ.pop();
    }

    assert_interrupts_disabled();
};

void cv::impl::broadcast() {
    assert_interrupts_disabled();

    // wake up all threads
    while (!waitingQ.empty()) {
        readyQ.push(waitingQ.front());
        waitingQ.pop();
    }

    assert_interrupts_disabled();
};