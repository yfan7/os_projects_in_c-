#ifndef _THREADIMPL_H
#define _THREADIMPL_H

#include "thread.h"
#include "globals.h"
#include <queue>

class thread::impl {
    public:
        impl(thread_startfunc_t, void *);
        ~impl();

        void join();
        static void starter(thread_startfunc_t func, void * arg, thread::impl* thread_class);
    private:
        int thread_ID;
};


#endif //_THREADIMPL_H