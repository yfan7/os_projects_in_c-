#include <iostream>
#include <cstdlib>
#include "thread.h"

using std::cout;
using std::endl;

int var1 = 0;
int var2 = 0;

mutex mutex1;
mutex mutex2;
cv cv1;

void child1(void *a)
{
    mutex1.lock();
    assert_interrupts_enabled();
    mutex2.lock();
    assert_interrupts_enabled();

    while (var1 < 5) {
        cv1.wait(mutex1);
        assert_interrupts_enabled();
    }
    while (var2 < 5) {
        cv1.wait(mutex2);
        assert_interrupts_enabled();
    }
    cout << "child 1 finshed!" << endl;
    mutex2.unlock();
    assert_interrupts_enabled();
    mutex1.unlock();
    assert_interrupts_enabled();

}

void child2(void *a) 
{
    mutex1.lock();
    assert_interrupts_enabled();
    for (int i = 0; i < 6; i++) {
        var1++;
        var2++;
        cout << "var1 = " << var1 << endl;
        cout << "var2 = " << var2 << endl;
    }
    cv1.signal();
    assert_interrupts_enabled();
    mutex1.unlock();
    assert_interrupts_enabled();

}

void parent(void *a)
{
    thread t1 ((thread_startfunc_t) child1, (void *) "child thread 1");
    assert_interrupts_enabled();
    thread t2 ((thread_startfunc_t) child2, (void *) "child thread 2");
    assert_interrupts_enabled();

}

int main()
{
    cpu::boot(1, (thread_startfunc_t) parent, (void *) 100, false, false, 0);
}