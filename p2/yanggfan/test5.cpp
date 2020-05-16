#include <iostream>
#include <cstdlib>
#include "thread.h"

using std::cout;
using std::endl;

int var = 0;

mutex m1;
cv cv1;
cv cv2;

void child1(void *a)
{
    assert_interrupts_enabled();
    m1.lock();
    assert_interrupts_enabled();
    while (var < 1) {
        cout << "child1 waiting..." << endl;
        assert_interrupts_enabled();
        cv1.wait(m1);
        assert_interrupts_enabled();
    }
    var++;
    assert_interrupts_enabled();
    cv2.signal();
    assert_interrupts_enabled();
    cout << "child1 finished..." << endl;
    assert_interrupts_enabled();
    m1.unlock();
    assert_interrupts_enabled();
}

void child2(void *a)
{
    assert_interrupts_enabled();
    m1.lock();
    assert_interrupts_enabled();
    while (var < 1) {
        cout << "child2 waiting..." << endl;
        assert_interrupts_enabled();
        cv2.wait(m1);
        assert_interrupts_enabled();
    }
    var++;
    assert_interrupts_enabled();
    cv1.signal();
    assert_interrupts_enabled();
    cout << "child2 finished..." << endl;
    assert_interrupts_enabled();
    m1.unlock();
    assert_interrupts_enabled();
}

void parent(void *a)
{
    assert_interrupts_enabled();
    thread t1 ( (thread_startfunc_t) child1, NULL);
    assert_interrupts_enabled();
    thread t2 ( (thread_startfunc_t) child2, NULL);
    assert_interrupts_enabled();

}

int main()
{
    assert_interrupts_enabled();
    cpu::boot(1, (thread_startfunc_t) parent, (void *) 100, false, false, 0);
    assert_interrupts_enabled();
}