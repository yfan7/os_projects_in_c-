#include <iostream>
#include <cstdlib>
#include "thread.h"

using std::cout;
using std::endl;

mutex mutex1, m2;
cv cv1, cv2, cv3;
int count = 10;
int count2 = 5;
bool increaser_done = true;

static thread* s1;
static thread* s2;
static thread* s3;
static thread* s4;
void child(void *a) {
    mutex1.lock();
    assert_interrupts_enabled();
    cv1.wait(mutex1);
    assert_interrupts_enabled();
    cv1.wait(m2);
    assert_interrupts_enabled();
}
void child2(void *a) {
    assert_interrupts_enabled();
    m2.lock();
    assert_interrupts_enabled();

}
void child3(void *a) {
    assert_interrupts_enabled();
    s3->join();
    assert_interrupts_enabled();

}
void parent(void *a) 
{
    cout << "parent before yield\n";
    thread::yield();
    assert_interrupts_enabled();
    cout << "parent after yield\n";
    int number1(1);
    s1 = new thread((thread_startfunc_t) child, (void *) &number1);
    assert_interrupts_enabled();
    s1->join();
    assert_interrupts_enabled();
    s2= new thread( (thread_startfunc_t) child2, (void *) &number1);
    assert_interrupts_enabled();
    s2->join();
    assert_interrupts_enabled();
    s3 = new thread( (thread_startfunc_t) child, (void *) &number1);
    assert_interrupts_enabled();
    s4 = new thread( (thread_startfunc_t) child, (void *) &number1);
    assert_interrupts_enabled();

    cout << "before join t4\n";
   
    cout << "parent finished\n";
}

int main()
{
    cpu::boot(1, (thread_startfunc_t) parent, (void *) 100, false, false, 0);
    
}