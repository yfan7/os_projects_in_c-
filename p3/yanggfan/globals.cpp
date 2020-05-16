#include "globals.h"
#include "vm_arena.h"


bool *used_in_swap_file;
int num_physmem_page;
int num_used_physmem_page;
int num_swap_blocks;
int num_used_swap_blocks;
pid_t running_process;
bool *used_memory;
const int zero_filled_page = 0;
std::unordered_map<pid_t, process_info> process_info_map;
std::deque<page_info_bits*> lru_clock;
std::unordered_map<std::pair<std::string, size_t>, page_info_bits*, hash_pair> file_backed_map;

// allocate a physical page and return physical page number
// evict virtual page (using lru_clock) if no physical pages are free
int find_physical_page() {
    int mem_page = 0;
 
    if (num_used_physmem_page == num_physmem_page) { // physical memory full
        //find the first unreferenced ppage
        while (lru_clock.front()->referenced){
            lru_clock.front()->referenced = false;

            // set read and write enable to false to trigger fault on next read/write to update referenced bit
            update_read_write_bits(lru_clock.front());
            
            // move the lru clock head to the next page
            lru_clock.push_back(lru_clock.front());
            lru_clock.pop_front();
        }

        unsigned int evicted_ppage = lru_clock.front()->ppage;
        lru_clock.front()->resident = false;
    
        // write back to disk if the page about to be evicted is dirty
        if (lru_clock.front()->dirty) {
            unsigned int buff = evicted_ppage * VM_PAGESIZE;
            file_write((const char*)lru_clock.front()->filename,
                       lru_clock.front()->block,
                       (void*)&((char*)vm_physmem)[buff]);
            lru_clock.front()->dirty = false;
        }
        
        update_read_write_bits(lru_clock.front());

        
        // delete info bits for page if no process is using this page
        bool delete_info_bits = true;
        if (lru_clock.front()->filename) {
            auto it = lru_clock.front()->ptable_entries.begin();
            while (it != lru_clock.front()->ptable_entries.end()) {
                //if any one process is using this file and block, then keep the pair in file_backed_map
                if (it->second) {
                    delete_info_bits = false;
                    ++it;
                }
                else {
                    //clean up redundant ptable_entries
                    lru_clock.front()->ptable_entries.erase(it++);
                }
            }

            if (delete_info_bits) { // no page table entries using this page
                file_backed_map.erase(std::make_pair(std::string(
                                      lru_clock.front()->filename), 
                                      lru_clock.front()->block));
                delete[] lru_clock.front()->filename;
                
                // delete info bits
                delete lru_clock.front();
                lru_clock.front() = nullptr;
            }
        }
        // evit the page from lru clock        
        mem_page = evicted_ppage;
        lru_clock.pop_front();
    }
    else { // no eviction needed
        // find next available page in physical memory
        while (used_memory[mem_page]) ++mem_page;

        // mark page as used
        used_memory[mem_page] = true;
        num_used_physmem_page++;    
    }
    
    // return physical page number of allocated page
    return mem_page;
}

// update the read and write bits based on the state of the info bits for the page
void update_read_write_bits(page_info_bits* bts) {
    if (bts->referenced && bts->resident && bts->dirty) {
        for (auto i : bts->ptable_entries){
            if (i.second){
                i.first->read_enable = 1;
                i.first->write_enable = 1;
            }
        }
        bts->read_enable = 1;
        bts->write_enable = 1;
    }
    // including the case when a swap-backed page is mapped to 0-filled page
    else if (bts->referenced && bts->resident && !bts->dirty) {
        for (auto i : bts->ptable_entries) {
            if (i.second){
                i.first->read_enable = 1;
                i.first->write_enable = 0;
            }
        }
        bts->read_enable = 1;
        bts->write_enable = 0;
    }
    else {
        for (auto i : bts->ptable_entries){
            if (i.second) {
                i.first->read_enable = 0;
                i.first->write_enable = 0;
            }
        }
        bts->read_enable = 0;
        bts->write_enable = 0;

    }
    
}
