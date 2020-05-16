#include "cv.h"
#include "cvImpl.h"
#include "cpu.h"
#include <stdexcept>
#include "globals.h"

cv::cv() {
    interrupt_handler handler;
    impl_ptr = new impl();
};

cv::~cv() {
    interrupt_handler handler;
    delete impl_ptr;
};

void cv::wait(mutex& m) {
    interrupt_handler handler;
    try {
        impl_ptr->wait(m);
    }
    catch (std::runtime_error e) {
        throw e;
    }
};

void cv::signal() {
    interrupt_handler handler;
    impl_ptr->signal();
};

void cv::broadcast() {
    interrupt_handler handler;
    impl_ptr->broadcast();
};