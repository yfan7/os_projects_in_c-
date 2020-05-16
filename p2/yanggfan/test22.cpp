#include <iostream>
#include "thread.h"
mutex x, y, c;

void child1()
{
for (int i=0; i<1000000; i++) {
    if (!(i%100)) {
        c.lock();
        std::cout << "child1: " << i << std::endl;
        c.unlock();
    }
        x.lock();
        y.lock();
        y.unlock();
        x.unlock();
    }
}

void child2()
{
for (int i=0; i<1000000; i++) {
    if (!(i%100)) {
        c.lock();
        std::cout << "child2: " << i << std::endl;
        c.unlock();
    }
    x.lock();
    y.lock();
    x.unlock();
    y.unlock();
    }
}

void parent(){
    // thread* t[2];
    thread t1((thread_startfunc_t) child1, nullptr);
    thread t2((thread_startfunc_t) child2, nullptr);
    t1.join();
    t2.join();
}




int main()
{
        cpu::boot(1, (thread_startfunc_t) parent, (void *) 100, false, false, 0);
}
