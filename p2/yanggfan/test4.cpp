#include <iostream>
#include <cstdlib>
#include "thread.h"

using std::cout;
using std::endl;

mutex mutex1, m2;
cv cv1, cv2;
int count = 10;
int count2 = 5;
void decreaser(void *a)
{
    cout << "decreaser started\n";
    assert_interrupts_enabled();
    mutex1.lock();
    assert_interrupts_enabled();
    cout << (char *)a << endl; 
    while(count > 0){
        count--;
        cout << "decreaser count " << count <<endl;
        assert_interrupts_enabled();
        cv1.broadcast();
        assert_interrupts_enabled();
    }
    assert_interrupts_enabled();
    mutex1.unlock();
    assert_interrupts_enabled();
    cout << "decreaser finished\n";
}

void child(void *a)
{
    assert_interrupts_enabled();
    mutex1.lock();
    assert_interrupts_enabled();
    while(count > 0){
        assert_interrupts_enabled();
        cv1.wait(mutex1);
        assert_interrupts_enabled();
        cout << "child before yield\n" ;
        assert_interrupts_enabled();
        thread::yield();
        assert_interrupts_enabled();
        cout << "child after yield\n" ;
    }
    cout << "child finished of thread " << *((int *) a) << endl;
    assert_interrupts_enabled();
    mutex1.unlock();
    assert_interrupts_enabled();
}


void child_variant(void *a)
{
    
    cout << "variant before yield\n" ;
    assert_interrupts_enabled();
    thread::yield();
    assert_interrupts_enabled();
    cout << "variant after yield\n" ;
    while(count2 > 0){
        int number = 3;
        assert_interrupts_enabled();
        thread t3( (thread_startfunc_t) child, (void *) & number);
        assert_interrupts_enabled();
        cout << "before join t3\n";
        assert_interrupts_enabled();
        t3.join();
        assert_interrupts_enabled();
        cout << "after join t3\n";
        count = 10;
        assert_interrupts_enabled();
        thread t6( (thread_startfunc_t) decreaser, (void *) "decreaser");
        assert_interrupts_enabled();
        count2--;
    }
    cout << "variant finished\n" ;
    

}


void parent(void *a) 
{
    cout << "parent before yield\n";
    assert_interrupts_enabled();
    thread::yield();
    assert_interrupts_enabled();
    cout << "parent after yield\n";
    int number1(1), number2(2);
    assert_interrupts_enabled();
    thread t1( (thread_startfunc_t) child, (void *) &number1);
    assert_interrupts_enabled();
    thread t2( (thread_startfunc_t) child, (void *) &number2);
    assert_interrupts_enabled();
    thread t5( (thread_startfunc_t) decreaser, (void *) "child thread of parent 5");
    assert_interrupts_enabled();
    thread t4( (thread_startfunc_t) child_variant, (void *) "child thread of parent variant");
    assert_interrupts_enabled();
    cout << "before join t4\n";
    t4.join();
    assert_interrupts_enabled();
    cout << "parent finished\n";
}


int main()
{
    assert_interrupts_enabled();
    cpu::boot(1, (thread_startfunc_t) parent, (void *) 100, false, false, 0);
    assert_interrupts_enabled();
}