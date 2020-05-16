#include <iostream>
#include <cstdlib>
#include "thread.h"

using std::cout;
using std::endl;

int g = 0;

mutex mutex1;
cv cv1;

void loop(void *a)
{
    char *id = (char *) a;
    int i;
    assert_interrupts_enabled();
    mutex1.lock();
    cout << "loop called with id " << id << endl;
    assert_interrupts_enabled();
    for (i=0; i<5; i++, g++) {
	cout << id << ":\t" << i << "\t" << g << endl;
        assert_interrupts_enabled();
        mutex1.unlock();
        assert_interrupts_enabled();
	    thread::yield();
        assert_interrupts_enabled();
        mutex1.lock();
        assert_interrupts_enabled();
    }
    cout << id << ":\t" << i << "\t" << g << endl;
    assert_interrupts_enabled();
    mutex1.unlock();
    assert_interrupts_enabled();
}

void parent(void *a)
{
        assert_interrupts_enabled();
    intptr_t arg = (intptr_t) a;
assert_interrupts_enabled();
    mutex1.lock();
    assert_interrupts_enabled();
    cout << "parent called with arg " << arg << endl;
    mutex1.unlock();
assert_interrupts_enabled();
    thread t1 ( (thread_startfunc_t) loop, (void *) "child thread");
assert_interrupts_enabled();
    loop( (void *) "parent thread");
}

int main()
{
    cpu::boot(1, (thread_startfunc_t) parent, (void *) 100, false, false, 0);
}