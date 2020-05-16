#include <iostream>
#include <cstdlib>
#include "thread.h"
#include <exception>

using std::cout;
using std::endl;

mutex mutex1;

void child(void *a)
{
    cout << "trying to unlock mutex\n";
    try {
        mutex1.unlock();
    }
    catch (std::exception e) {
        assert_interrupts_enabled();
        cout << e.what() << endl;
    }
    cout << "finished trying to unlock mutex\n";

}


void parent(void *a) 
{
    mutex1.lock();
    cout << "I am parent" << endl;

    thread t1 ( (thread_startfunc_t) child, (void *) "child thread of parent 2");
    t1.join();

    cout << "Child thread finished!" << endl;
    mutex1.unlock();
}


int main()
{
    cpu::boot(1, (thread_startfunc_t) parent, (void *) 100, false, false, 0);
}