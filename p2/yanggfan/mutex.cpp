#include "mutex.h"
#include "mutexImpl.h"
#include "cpu.h"
#include <stdexcept>
#include "globals.h"

mutex::mutex(){
    interrupt_handler handler;
    impl_ptr = new impl();
}

mutex::~mutex() {
    interrupt_handler handler;
    delete impl_ptr;
}

void mutex::lock() {
    interrupt_handler handler;
    impl_ptr->lock();
}

void mutex::unlock() {
    interrupt_handler handler;
    try {
        impl_ptr->unlock();
    }
    catch (std::runtime_error e) {
        throw e;
    }
}

