#include <iostream>
#include <cstdlib>
#include "thread.h"

using std::cout;
using std::endl;

int index = 0;
const int MAX_INDEX = 99;
mutex mutex1;
cv cv1;

void child_odds() {
	assert_interrupts_enabled();
	mutex1.lock();
	assert_interrupts_enabled();
	while (index < MAX_INDEX) {
		while (index % 2 == 0) {
			assert_interrupts_enabled();
			cv1.wait(mutex1);
			assert_interrupts_enabled();
		}
		cout << index << " ";
		index++;
		if (index % 10 == 0) cout << "\n";
		assert_interrupts_enabled();
		cv1.signal();
		assert_interrupts_enabled();
	}
	assert_interrupts_enabled();
	mutex1.unlock();
	assert_interrupts_enabled();
}

void child_evens() {
	assert_interrupts_enabled();
	mutex1.lock();
	assert_interrupts_enabled();
	while (index < MAX_INDEX) {
		while (index % 2 == 1) {
			assert_interrupts_enabled();
			cv1.wait(mutex1);
			assert_interrupts_enabled();		}
		cout << index << " ";
		index++;
		if (index % 10 == 0) cout << "\n";
		assert_interrupts_enabled();
		cv1.signal();
		assert_interrupts_enabled();
	}
	assert_interrupts_enabled();
	mutex1.unlock();
	assert_interrupts_enabled();
}

void parent(void *a)
{
	assert_interrupts_enabled();
	mutex1.lock();
	assert_interrupts_enabled();
	cout << "parent started. Index = " << index << endl;
	assert_interrupts_enabled();
	mutex1.unlock();
	assert_interrupts_enabled();
	assert_interrupts_enabled();
	thread t1((thread_startfunc_t)child_evens, (void*)100);
	assert_interrupts_enabled();
	thread t2((thread_startfunc_t)child_odds, (void*)100);
	assert_interrupts_enabled();


	cout << "\njoining\n";
	assert_interrupts_enabled();
	t1.join();
	assert_interrupts_enabled();
	t2.join();
	assert_interrupts_enabled();
	cout << "\ndone joining\n";
	assert_interrupts_enabled();
	mutex1.lock();
	assert_interrupts_enabled();
	cout << "all child threads finished. Index = " << index << endl;
	assert_interrupts_enabled();
	mutex1.unlock();
	assert_interrupts_enabled();
}

int main()
{	
	assert_interrupts_enabled();
	cpu::boot(1, (thread_startfunc_t)parent, (void *)100, false, false, 0);
	assert_interrupts_enabled();
}