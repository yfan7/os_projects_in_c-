#include <iostream>
#include <cstdlib>
#include "thread.h"

using std::cout;
using std::endl;

mutex m1, m2;
cv c1, c2;
int num;
int num2;

void child2(void *a)
{
    //m3.unlock();
    m2.lock();
    assert_interrupts_enabled();
    while (num2 < 30)
    {
        assert_interrupts_enabled();

        cout << "****" << num2 << "\n";
        ++num2;
        c2.signal();
        int index = num2;
        while (index == num2 && num2 != 30)
        {
            c2.wait(m2);
        }
        thread::yield();
        assert_interrupts_enabled();

        thread::yield();

        assert_interrupts_enabled();
    }
    m2.unlock();
    cout << "thread finished\n";
}

void child1(void *a)
{
    //m3.unlock();
    m1.lock();
    assert_interrupts_enabled();
    while (num < 10)
    {

        int index = num + 4;
        assert_interrupts_enabled();
        ++num;
        cout << num << "\n";
        c1.signal();
        assert_interrupts_enabled();

        thread tt1((thread_startfunc_t)child2, NULL);
        assert_interrupts_enabled();

        thread tt2((thread_startfunc_t)child2, NULL);
        assert_interrupts_enabled();

        thread tt3((thread_startfunc_t)child2, NULL);
        assert_interrupts_enabled();

        tt1.join();
        assert_interrupts_enabled();

        tt2.join();
        assert_interrupts_enabled();

        tt3.join();
        assert_interrupts_enabled();

        num2 = 0;

        while (index != num)
        {
            c1.wait(m1);
            assert_interrupts_enabled();
        }
        thread::yield();
        assert_interrupts_enabled();

        thread::yield();
        assert_interrupts_enabled();

        assert_interrupts_enabled();
    }
    m1.unlock();
}

void parent(void *a)
{
    num = 0;
    thread t1((thread_startfunc_t)child1, NULL);
    assert_interrupts_enabled();
    thread t2((thread_startfunc_t)child1, NULL);
    assert_interrupts_enabled();
    thread t3((thread_startfunc_t)child1, NULL);
    assert_interrupts_enabled();
    thread t4((thread_startfunc_t)child1, NULL);
    assert_interrupts_enabled();
}

int main()
{
    cpu::boot(1, (thread_startfunc_t)parent, (void *)100, false, false, 0);
}