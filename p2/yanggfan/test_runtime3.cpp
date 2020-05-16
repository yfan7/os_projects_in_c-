#include <iostream>
#include <cstdlib>
#include "thread.h"
#include <stdexcept>
using std::cout;
using std::endl;

mutex mutex1;
cv cv1;
void parent(void *a) 
{
    cout << "trying to wait on unlocked mutex\n";
    try {
        cv1.wait(mutex1);
    }
    catch (std::exception e) {
        assert_interrupts_enabled();
        cout << e.what() << endl;
    }
    cout << "finished trying to wait on unlocked mutex\n";
}


int main()
{
    cpu::boot(1, (thread_startfunc_t) parent, (void *) 100, false, false, 0);
}