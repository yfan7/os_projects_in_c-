#ifndef _MUTEXIMPL_H
#define _MUTEXIMPL_H

#include "mutex.h"

#include <queue>
#include <ucontext.h>
#include "globals.h"

class mutex::impl {
    public:
        impl();

        ~impl();

        void lock(); 
        void unlock();

    private:
        std::queue<context_with_ID> mutexQ;
        bool locked;
        int owner_ID;

};


#endif //_CPUIMPL_H