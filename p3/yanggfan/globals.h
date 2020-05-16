#ifndef _GLOBALS_H_
#define _GLOBALS_H_
#include <unordered_map>
#include <unordered_set>
#include <deque>
#include "vm_app.h"
#include "vm_pager.h"
#include <string>

// hash function for unordered_map whose key is a pair
struct hash_pair { 
    template <class T1, class T2> 
    size_t operator()(const std::pair<T1, T2>& p) const
    { 
        auto hash1 = std::hash<T1>{}(p.first); 
        auto hash2 = std::hash<T2>{}(p.second); 
        return hash1 ^ hash2; 
    } 
}; 

// struct for info bits for each virtual page entry
struct page_info_bits {
    bool valid = false; 
    bool resident = false; // page loaded in physical memory
    bool dirty = false;
    bool referenced = false;
    bool zero_filled = false; // page pointing to zero_filled_page
    char* filename = nullptr;
    int block = -1;

    unsigned int ppage; // physical page number
    unsigned int read_enable;
    unsigned int write_enable;

    // pointers to all page table entries that have the same filename and block
    // so as to change all the read and write enable bits
    // bool used to mark if a process has finished using this page table entry or not
    // bool set to false during vm_destroy (when a process ends)
    std::unordered_set<std::pair<page_table_entry_t*, bool> , hash_pair> ptable_entries;

};


struct process_info {
    void* next_available_addr;
    page_table_t* page_table;
    page_info_bits** info_bits;
};

extern bool *used_memory; // mark physical memory pages as used
extern int num_physmem_page; // total number of physical pages
extern int num_used_physmem_page; // number of used physical memory pages

extern bool *used_in_swap_file; // mark swap blocks in swap file as used
extern int num_swap_blocks; // total number of swap blocks
extern int num_used_swap_blocks; // number of used swap blocks

extern pid_t running_process; //the current running process id

//ppage number of zero filled page
extern const int zero_filled_page;

// map a process id to the struct with page table and info bits array
extern std::unordered_map<pid_t, process_info> process_info_map;

extern std::deque<page_info_bits*> lru_clock;

//for allowing different processes share the same physical page and file block
extern std::unordered_map<std::pair<std::string, size_t>, page_info_bits*, hash_pair> file_backed_map;

// allocate a physical page and return physical page number
// evict virtual page (using lru_clock) if no physical pages are free
extern int find_physical_page();

// update the read and write bits based on the state of the info bits for the page
extern void update_read_write_bits(page_info_bits*);
#endif 