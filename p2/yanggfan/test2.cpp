#include <iostream>
#include <fstream>
#include "cpu.h"
#include "cv.h"
#include "thread.h"
#include <vector>
#include <limits>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <algorithm>

using std::vector;
using std::pair;
using std::numeric_limits;
using std::ifstream;
using std::min;
using std::make_pair;
using std::cin;
using std::cout;
 
const int NUM_THREADS = 5;
int MAX_QUEUE_SIZE = 0;
int current_track = 0;
int operating_threshold;
mutex m;
mutex m_update_threadhold;

cv check;
cv run;
vector<pair<int,int> > queue;
vector<bool> dup;


void print_request(unsigned int requester, unsigned int track) {
    assert_interrupts_enabled();
    cout << "requester " << requester << " track " << track << "\n";
}
void print_service(unsigned int requester, unsigned int track)  {
    cout << "service requester " << requester << " track " << track << "\n";
}
vector<vector<int>> disks { {785, 53}, {350, 914}, {827, 567}, {302, 230}, {631, 11} };
void requester(void *a){
    pair<char*,int >* p = (pair<char*,int > *)a;
    // char * filename = p->first;
    // ifstream fs(filename);
    int requester_num = p->second;
    int first_on_wait;
    // while (fs >> first_on_wait ){
    for (int i = 0; i < 2; i++){
        first_on_wait = disks[requester_num][i];
        assert_interrupts_enabled();
        m.lock();
        assert_interrupts_enabled();
        while(dup[(size_t)requester_num] || (int)queue.size() == MAX_QUEUE_SIZE){
            assert_interrupts_enabled();
            check.wait(m);
            assert_interrupts_enabled();
        }
        queue.push_back(make_pair(first_on_wait, requester_num));
        print_request(requester_num, first_on_wait);
        dup[(size_t)requester_num] = true;
        assert_interrupts_enabled();
        run.signal();
        assert_interrupts_enabled();
        m.unlock();
        assert_interrupts_enabled();
    }
    assert_interrupts_enabled();
    m.lock();
    assert_interrupts_enabled();
    while(dup[(size_t)requester_num]){
        check.wait(m);
    }
    operating_threshold--;
    assert_interrupts_enabled();
    run.signal();
    assert_interrupts_enabled();
    m.unlock();
    assert_interrupts_enabled();
    delete p;
    p = nullptr;
    a = nullptr;
}

void servicer(void *a){
    while (true){
        assert_interrupts_enabled();
        m.lock();
        assert_interrupts_enabled();
        while ((int)queue.size() < min(operating_threshold, MAX_QUEUE_SIZE)) {
            assert_interrupts_enabled();
            run.wait(m);
            assert_interrupts_enabled();
        }
        if (operating_threshold == 0 && queue.empty()) return;
            int distance = numeric_limits<int>::max();
            int requester_number = 0;
            int j = 0;
            int cur_j = 0;
            for (auto i : queue){
                
                if (abs(i.first - current_track ) < distance) {
                    distance = abs(i.first - current_track);
                    requester_number = i.second;
                    cur_j = j;
                }
                j++;
            }
            current_track = queue[cur_j].first;
            print_service(requester_number, current_track);
            dup[requester_number] = false;
            queue.erase(queue.begin()+cur_j);
            assert_interrupts_enabled();
            check.broadcast();
            assert_interrupts_enabled();
        m.unlock();
        assert_interrupts_enabled();


    }
    


}
pair<int, char**> pp;
void parent(void *a){
    MAX_QUEUE_SIZE = 3;
    thread *t[NUM_THREADS];

    operating_threshold = NUM_THREADS;

    for (int i=0; i<NUM_THREADS; i++) {
        dup.push_back(false);

    }
    for (int i=0; i<NUM_THREADS; i++) {
        pair<char*,int >* temp = new pair<char*,int >(nullptr,i);

        t[i] = new thread(requester, temp);
    }
    thread s(servicer,NULL);
    s.join();

    for (int i=0; i<NUM_THREADS; i++) {
        t[i]->join();
    }

}

int main()
{
    pp.first = 5;
    cpu::boot(1, (thread_startfunc_t) parent, (void *) 100, false, false, 0);
    return 0;
}