#ifndef _GLOBALS_H
#define _GLOBALS_H

#include <ucontext.h>
#include <queue>
#include <vector>
#include <unordered_map>
#include "cpu.h"

struct context_with_ID {
    ucontext_t* context_ptr;
    int ID;
};

extern std::queue<context_with_ID> readyQ;

extern context_with_ID running_context;

extern int new_ID;

extern std::vector<bool> finished_threads;

extern char* old_stack;

extern std::unordered_map<int, std::queue<context_with_ID>*> joinQ;

extern void switch_to_next_ready_thread();

// RAII for disabling and enabling interrupts
class interrupt_handler {
    public:
    interrupt_handler();
    ~interrupt_handler();
};

#endif
