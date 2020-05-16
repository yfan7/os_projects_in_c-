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
    delete s1;
    assert_interrupts_enabled();
  
    assert_interrupts_enabled();
    
    assert_interrupts_enabled();
    cout << "before join s2 in child\n";
    s2->join();
    cout << "after join s2 in child\n";
    mutex1.unlock();
}
void child4(void *a) {
    mutex1.lock();
    // delete s4;
    assert_interrupts_enabled();
  
    assert_interrupts_enabled();
    
    assert_interrupts_enabled();
    cout << "before join s2 in child\n";
    // s2->join();
    cout << "after join s2 in child\n";
    mutex1.unlock();
}
void child2(void *a) {
    cout << "before join s3 in child2\n";
    s3->join();
    cout << "after join s3 in child2\n";
    assert_interrupts_enabled();
    assert_interrupts_enabled();
}
void child3(void *a) {
    assert_interrupts_enabled();
    cout << "before join s4 in child3\n";
    s4->join();
    cout << "after join s4 in child3\n";
    assert_interrupts_enabled();

}
void parent(void *a) 
{
    cout << "parent before yield\n";
    thread::yield();
    assert_interrupts_enabled();
    cout << "parent after yield\n";
    int number1(1);
    s4 = new thread( (thread_startfunc_t) child4, (void *) &number1);
    s3 = new thread( (thread_startfunc_t) child3, (void *) &number1);
    s2= new thread( (thread_startfunc_t) child2, (void *) &number1);
    s1 = new thread((thread_startfunc_t) child, (void *) &number1);
    assert_interrupts_enabled();
    cout << "before join s1\n";
    
    cout << "after join s1\n";
    assert_interrupts_enabled();
    
    assert_interrupts_enabled();
    cout << "before delete s2\n";
    s1->join();
    delete s2;
    cout << "after delete s2\n";
    assert_interrupts_enabled();
    
    delete s3;
    assert_interrupts_enabled();
    
    assert_interrupts_enabled();
    //delete s4;

    cout << "before join t4\n";
   
    cout << "parent finished\n";
}

int main()
{
    cpu::boot(1, (thread_startfunc_t) parent, (void *) 100, false, false, 0);
    
}