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
void decreaser(void *a)
{
    cout << "decreaser started\n";
    mutex1.lock();
    assert_interrupts_enabled();

    cout << (char *)a << endl; 
    while(count > 0){
        count--;
        cout << "decreaser count " << count <<endl;
        cv1.broadcast();
        assert_interrupts_enabled();
    }
    mutex1.unlock();
    assert_interrupts_enabled();
    cout << "decreaser finished\n";
}
void waited_child(void *a);

void waiting_child(void *a);

void child(void *a)
{
    
    mutex1.lock();
    assert_interrupts_enabled();
    
    while(count > 0){
        cv1.wait(mutex1);
        assert_interrupts_enabled();
        cout << "child before yield\n" ;
   
        cout << "child after yield\n" ;
    }
    cout << "child finished of thread " << *((int *) a) << endl;
    mutex1.unlock();
    assert_interrupts_enabled();
}
void increaser(void * a){
    m2.lock();
    assert_interrupts_enabled();
    while(count <10 ){
        cv3.wait(m2);
        assert_interrupts_enabled();

        cv2.signal();
        assert_interrupts_enabled();
        count++;
        int number(0);
        thread t( (thread_startfunc_t) child, (void *) & number);
        assert_interrupts_enabled();
        cout << "before join t\n" ;
        t.join();
        assert_interrupts_enabled();
        cout << "after join t\n" ;
    }
    increaser_done = true;
    m2.unlock();
    assert_interrupts_enabled();
}
void child_variant(void *a)
{
    
    cout << "variant before yield\n" ;
    thread::yield();
    assert_interrupts_enabled();
    cout << "variant after yield\n" ;
    while(count2 > 0){
        int number = 3;
        increaser_done = false;
        thread t3( (thread_startfunc_t) child, (void *) & number);
        assert_interrupts_enabled();
        thread t7( (thread_startfunc_t) increaser, (void *) & number);
        assert_interrupts_enabled();
        cout << "before broadcast loop\n";
        while(!increaser_done){
            m2.lock();
            assert_interrupts_enabled();
            cout << "after variant gets m2 loop\n";
            if (!increaser_done){
                cv3.broadcast();
                assert_interrupts_enabled();
                cv2.wait(m2);
                assert_interrupts_enabled();

            }
            m2.unlock();
            assert_interrupts_enabled();
            cout << "after variant releases m2 loop\n";
        }
        cout << "after broadcast loop\n";
        thread t6( (thread_startfunc_t) decreaser, (void *) "decreaser");
        assert_interrupts_enabled();
        count2--;
    }
    cout << "variant finished\n" ;
    

}


void parent(void *a) 
{
    cout << "parent before yield\n";
    thread::yield();
    assert_interrupts_enabled();
    cout << "parent after yield\n";
    int number1(1), number2(2);
    s1 = new thread((thread_startfunc_t) child, (void *) &number1);
    assert_interrupts_enabled();    
    s2= new thread( (thread_startfunc_t) child, (void *) &number1);
    assert_interrupts_enabled();
    s3 = new thread( (thread_startfunc_t) child, (void *) &number1);
    assert_interrupts_enabled();
    s4 = new thread( (thread_startfunc_t) child, (void *) &number1);
    assert_interrupts_enabled();
    thread t1( (thread_startfunc_t) waited_child, (void *) &number1);
    assert_interrupts_enabled();
    thread t5( (thread_startfunc_t) decreaser, (void *) "child thread of parent 5");
    assert_interrupts_enabled();    
    thread t4( (thread_startfunc_t) child_variant, (void *) "child thread of parent variant");
    assert_interrupts_enabled();
    thread t2( (thread_startfunc_t) waiting_child, (void *) &number2);
    assert_interrupts_enabled();

    cout << "before join t4\n";
    t4.join();
    assert_interrupts_enabled();
    cout << "parent finished\n";
}


void waited_child(void *a){

    s2->join();
    assert_interrupts_enabled();
    s3->join();
    assert_interrupts_enabled();
    s4->join();
    assert_interrupts_enabled();

}
void waiting_child(void *a){
    s1->join();
    assert_interrupts_enabled();

    mutex1.lock();
    assert_interrupts_enabled();

    while(count > 0){
        cv1.wait(mutex1);
        assert_interrupts_enabled();
        cout << "child before yield\n" ;
   
        cout << "child after yield\n" ;
    }
    cout << "child finished of thread " << *((int *) a) << endl;
    mutex1.unlock();
    assert_interrupts_enabled();
}
int main()
{
    cpu::boot(1, (thread_startfunc_t) parent, (void *) 100, false, false, 0);
    
}