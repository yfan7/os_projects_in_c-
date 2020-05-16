#ifndef _CPUIMPL_H
#define _CPUIMPL_H

#include "cpu.h"


class cpu::impl {
    public:
        impl(thread_startfunc_t func, void *arg);

        ~impl();

    private:
    

};


#endif //_CPUIMPL_H