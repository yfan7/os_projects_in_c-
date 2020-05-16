#include <iostream>
#include <cstdlib>
#include "thread.h"

using std::cout;
using std::endl;

mutex product_mutex;
cv supplier_waiting_cv;
cv consumer_waiting_cv;

int num_product;
int fill_count;

void supplier_func(void *a)
{   
    assert_interrupts_enabled();
    product_mutex.lock();
    assert_interrupts_enabled();

    while(fill_count < 10) {
        while(num_product > 0) {
            cout << "supplier waiting\n";
            assert_interrupts_enabled();
            supplier_waiting_cv.wait(product_mutex);
            assert_interrupts_enabled();

        }
        cout << "supplier refilled. fill_count: " << fill_count << "\n";
        num_product = 5;
        ++fill_count;
        assert_interrupts_enabled();
        consumer_waiting_cv.broadcast();
        assert_interrupts_enabled();

    }
    cout << "supplier finished\n";
    assert_interrupts_enabled();
    product_mutex.unlock();
    assert_interrupts_enabled();

}

void consumer_func(void *a)
{
    assert_interrupts_enabled();
    product_mutex.lock();
    assert_interrupts_enabled();

    while(num_product == 0) {
        cout << "consumer waiting\n";
        assert_interrupts_enabled();
        consumer_waiting_cv.wait(product_mutex);
        assert_interrupts_enabled();
    }
    cout << "consumer took product. num_product = " << num_product << "\n";
    --num_product;
    if(num_product == 0) {
        cout << "product empty. Calling supplier.\n";
        assert_interrupts_enabled();
        supplier_waiting_cv.signal();
        assert_interrupts_enabled();

    }
    assert_interrupts_enabled();
    product_mutex.unlock();
    assert_interrupts_enabled();

}

void parent(void *a)
{   
    assert_interrupts_enabled();
    product_mutex.lock();
    assert_interrupts_enabled();

    num_product = 5;
    assert_interrupts_enabled();
    thread supplier ( (thread_startfunc_t) supplier_func, NULL);
    assert_interrupts_enabled();

    for(int i = 0; i < 50; ++i) {
        assert_interrupts_enabled();
        thread consumer( (thread_startfunc_t) consumer_func, NULL);
        assert_interrupts_enabled();
    }
    assert_interrupts_enabled();
    product_mutex.unlock();
    assert_interrupts_enabled();
}

int main()
{
    cpu::boot(1, (thread_startfunc_t) parent, (void *) 100, false, false, 0);
}