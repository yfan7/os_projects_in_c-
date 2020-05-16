#include <iostream>
#include <cstdlib>
#include "thread.h"

using std::cout;
using std::endl;

mutex m;
cv c;

int num;

void child1(void *a)
{
    while(num < 100) {
        assert_interrupts_enabled();
        m.lock();
        assert_interrupts_enabled();
        while(num % 2 == 0) {
            assert_interrupts_enabled();
            c.wait(m);
            assert_interrupts_enabled();
        }
        ++num;
        cout << num << " ";
        if(num % 10 == 0) cout << "\n";
        assert_interrupts_enabled();
        m.unlock();
        assert_interrupts_enabled();
        c.signal();
        assert_interrupts_enabled();
    }
}

void child2(void *a)
{
    while(num < 100) {
        assert_interrupts_enabled();
        m.lock();
        assert_interrupts_enabled();
        while(num % 2 == 1) {
            assert_interrupts_enabled();
            c.wait(m);
            assert_interrupts_enabled();
        }
        ++num;
        cout << num << " ";
        if(num % 10 == 0) cout << "\n";
        assert_interrupts_enabled();
        m.unlock();
        assert_interrupts_enabled();
        c.signal();
        assert_interrupts_enabled();
    }
}

void parent(void *a)
{
    num = 0;

    assert_interrupts_enabled();
    thread t1( (thread_startfunc_t) child1, NULL);
    assert_interrupts_enabled();
    thread t2( (thread_startfunc_t) child2, NULL);
    assert_interrupts_enabled();

}

int main()
{
    assert_interrupts_enabled();
    cpu::boot(1, (thread_startfunc_t) parent, (void *) 100, false, false, 0);
    assert_interrupts_enabled();
}