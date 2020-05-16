#include <iostream>
#include <cstdlib>
#include "thread.h"

using std::cout;
using std::endl;

int g = 0;

mutex mutex1;
cv cv1;



void parent(void *a)
{

    assert_interrupts_enabled();
    cout << "I am parent\n";

}

int main()
{
    cpu::boot(1, (thread_startfunc_t) parent, (void *) NULL, false, false, 0);
}