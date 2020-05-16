#include <iostream>
#include <cstdlib>
#include "thread.h"

using std::cout;
using std::endl;

mutex m;
cv c;

bool run = false;
void child1(void *a)
{
    cout << "child 1 started\n";
    m.lock();
    assert_interrupts_enabled();
    while(!run) {
        c.wait(m);
        assert_interrupts_enabled();

    }
    cout << "child 1 exiting\n";

}

void child2(void *a)
{
    cout << "child 2 started\n";
    m.lock();
    assert_interrupts_enabled();
    while(!run) {
        c.wait(m);
        assert_interrupts_enabled();
    }
    cout << "child 2 exiting\n";

}

void parent(void *a)
{
    m.lock();
    assert_interrupts_enabled();

    thread* t1_pointer = nullptr;
    thread* t2_pointer = nullptr;

    thread t1( (thread_startfunc_t) child1, &t2_pointer);
    assert_interrupts_enabled();
    thread t2( (thread_startfunc_t) child2, &t1_pointer);
    assert_interrupts_enabled();

    t1_pointer = &t1;
    t2_pointer = &t2;
    
    run = true;
    c.broadcast();
    assert_interrupts_enabled();
    m.unlock();
    assert_interrupts_enabled();

    t1.join();
    assert_interrupts_enabled();
    t2.join();
    assert_interrupts_enabled();

}

int main()
{
    cpu::boot(1, (thread_startfunc_t) parent, (void *) 100, false, false, 0);
}