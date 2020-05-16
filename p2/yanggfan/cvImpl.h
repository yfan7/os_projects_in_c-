#ifndef _CVIMPL_H
#define _CVIMPL_H

#include "cv.h"
#include <ucontext.h>
#include <queue>
#include "globals.h"

class cv::impl {
    public:
        impl();

        ~impl();

        void wait(mutex&);                  
        void signal();                      
                                       
        void broadcast();    

    private:
        std::queue<context_with_ID> waitingQ;

};


#endif //_CPUIMPL_H