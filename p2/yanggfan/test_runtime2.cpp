#include <iostream>
#include <cstdlib>
#include "thread.h"
#include <stdexcept>
using std::cout;
using std::endl;

mutex mutex1;

void parent(void *a) 
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


int main()
{
    cpu::boot(1, (thread_startfunc_t) parent, (void *) 100, false, false, 0);
}