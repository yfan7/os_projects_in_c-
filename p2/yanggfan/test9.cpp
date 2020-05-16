#include <iostream>
#include <cstdlib>
#include "thread.h"
#include "globals.h"
// #include "cpu.h"
using std::cout;
using std::endl;

mutex m1;
int index;
void child(void *a) {
	assert_interrupts_enabled();
    m1.lock();
    assert_interrupts_enabled();
    //intptr_t id = (intptr_t) a;

    cout << "I am child of id " << index << endl;
    ++index;
    assert_interrupts_enabled();
    m1.unlock();
    assert_interrupts_enabled();
}


void parent(void *a)
{
	assert_interrupts_enabled();
    for (int i = 0; i < 100000; i++) {
    	assert_interrupts_enabled();
        try {
            thread t1 ((thread_startfunc_t) child, (void *) NULL);
            thread::yield();
        } 
        catch (std::exception e) {
            cout << e.what() << endl;
            break;
        }
        assert_interrupts_enabled();
        //cout << "readyQ.size(): " << readyQ.size() << endl;
    }

    thread t1 ((thread_startfunc_t) child, (void *) NULL);
    t1.join();

}

int main()
{
    cpu::boot(1, (thread_startfunc_t) parent, (void *) NULL, false, false, 0);
}