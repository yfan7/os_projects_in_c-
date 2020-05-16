#include "vm_pager.h"
#include <iostream>
#include "globals.h"
#include <cstring>
#include <utility>
#include <string>
using std::cout;


void vm_init(size_t memory_pages, size_t swap_blocks) {
    // initialize swap blocks
    num_swap_blocks = swap_blocks;
    used_in_swap_file = new bool[num_swap_blocks];
    for (int i = 0; i < num_swap_blocks; ++i) {
        used_in_swap_file[i] = false;
    }
    num_used_swap_blocks = 0;

    // initialize physical memory
    num_physmem_page = memory_pages;
    used_memory = new bool[memory_pages];
    for (size_t i = 0; i < memory_pages; ++i) {
        used_memory[i] = false;
    }

    // pinning the zero page 
    used_memory[zero_filled_page] = true;
    ++num_used_physmem_page;           
    for (unsigned int i = 0; i < VM_PAGESIZE/4; ++i) {
        ((unsigned int*)vm_physmem)[(zero_filled_page * VM_PAGESIZE)/4 + i] = 0;        
    }

};

int vm_create(pid_t parent_pid, pid_t child_pid) {
    // initialize page table for MMU
    page_table_t* p_table = new page_table_t;
    for (unsigned int i = 0; i < VM_ARENA_SIZE/VM_PAGESIZE; ++i) {
        p_table->ptes[i].read_enable = 0;
        p_table->ptes[i].write_enable = 0;
    }
    
    // initialize page info bits for kernel
    page_info_bits** info_bits = new page_info_bits*[VM_ARENA_SIZE/VM_PAGESIZE];
    for(unsigned int i = 0; i < VM_ARENA_SIZE/VM_PAGESIZE; ++i ) {
        info_bits[i] = nullptr;
    }
    process_info p_info2 = {VM_ARENA_BASEADDR, p_table, info_bits};

    // store the page info bits for corresponding child process    
    process_info_map[child_pid] = p_info2;

    return 0;
};

void *vm_map(const char *filename, size_t block) {

    // if arena is full return nullptr
    if ((char*)process_info_map[running_process].next_available_addr + VM_PAGESIZE > VM_ARENA_SIZE + (char*)VM_ARENA_BASEADDR)
        return nullptr;

    // get next lowest virtual page to be validated
    int page = ((char*)process_info_map[running_process].next_available_addr
                - (char*)VM_ARENA_BASEADDR)/VM_PAGESIZE;

    if (filename == nullptr) { // create new swap-backed page
        //check for available space on swap file
        if (num_swap_blocks == num_used_swap_blocks) return nullptr;
        
        // setting all the bits for the "mapped" state        
        process_info_map[running_process].info_bits[page] = new page_info_bits;
        process_info_map[running_process].info_bits[page]->ptable_entries.insert(
            std::make_pair(&process_info_map[running_process].page_table->ptes[page], 
            bool(true)));
        process_info_map[running_process].info_bits[page]->valid = true;
        process_info_map[running_process].info_bits[page]->resident = true;
        process_info_map[running_process].info_bits[page]->dirty = false;
        process_info_map[running_process].info_bits[page]->referenced = true;
        process_info_map[running_process].info_bits[page]->zero_filled = true;
        process_info_map[running_process].info_bits[page]->filename = nullptr;

        // find available swap block to reserve
        int block = 0;
        while (used_in_swap_file[block]) {
            ++block;
        }
        used_in_swap_file[block] = true;
        num_used_swap_blocks++;
        process_info_map[running_process].info_bits[page]->block = block;

        // set read_enable to 1 and physical page number to zero_filled_page
        for (auto it : process_info_map[running_process].info_bits[page]->ptable_entries){
            if(it.second) {
                it.first->ppage = zero_filled_page;
            }
        }
        process_info_map[running_process].info_bits[page]->ppage = zero_filled_page;

        update_read_write_bits(process_info_map[running_process].info_bits[page]);

        // return virtual address of validated page
        void* return_val = process_info_map[running_process].next_available_addr;
        process_info_map[running_process].next_available_addr = (char*)process_info_map[running_process].next_available_addr + VM_PAGESIZE;

        return return_val;
    } else { // file-backed page
        
        // if start of filename is outside valid arena return nullptr
        if (filename < VM_ARENA_BASEADDR || filename >= process_info_map[running_process].next_available_addr)
            return nullptr;

        // calculate virtual page number of offset of filename pointer
        int file_page = ((char*)filename - (char*)VM_ARENA_BASEADDR) / VM_PAGESIZE;
        int offset = (((char*)filename - (char*)VM_ARENA_BASEADDR) % VM_PAGESIZE);
        
        // string for the actual filename
        std::string phys_filename;
        
        while (true) { //bring filename to memory

            if (!process_info_map[running_process].info_bits[file_page]->read_enable) { // fault on read from file_page if necessary
                if(vm_fault((file_page * VM_PAGESIZE) + (char*)VM_ARENA_BASEADDR, false) == -1)
                    return nullptr;
            }
    
            char* file_page_phys_addr = 
                &((char *)vm_physmem)[(process_info_map[running_process].page_table->ptes[file_page].ppage * VM_PAGESIZE) + offset];
            int index = 0;
            //reach the end of page or null-terminating character
            while ((offset + index) < (int)VM_PAGESIZE && file_page_phys_addr[index]) {
                phys_filename += file_page_phys_addr[index];
                ++index;
            }
            
            //break when reaching null-terminating character
            if ((offset + index) < (int)VM_PAGESIZE) break;
            file_page++;
            offset = 0;

            //check if the next vpage is valid
            if ((file_page + 1) * VM_PAGESIZE > (char*)process_info_map[running_process].next_available_addr - (char*)VM_ARENA_BASEADDR)
                return nullptr;
        }
        
        //check if the filename c-string is entirely in the valid part of arena
        if (filename + phys_filename.length() + 1 > process_info_map[running_process].next_available_addr || filename + phys_filename.length() + 1 < filename)
            return nullptr;

        int mem_page = 0;
        if (file_backed_map.find(std::make_pair(phys_filename, block)) != file_backed_map.end()){ // filename and block already mapped
            process_info_map[running_process].info_bits[page] = 
                    file_backed_map[std::make_pair(phys_filename, block)];
            mem_page = process_info_map[running_process].info_bits[page]->ppage;
            
            // copy read write bits of the group
            process_info_map[running_process].page_table->ptes[page].read_enable = 
                    process_info_map[running_process].info_bits[page]->read_enable;
            process_info_map[running_process].page_table->ptes[page].write_enable =
                    process_info_map[running_process].info_bits[page]->write_enable;
            process_info_map[running_process].page_table->ptes[page].ppage = mem_page;
        }
        else { //create a new group of pages mapped to the same filename and block
         
            process_info_map[running_process].info_bits[page] = new page_info_bits;

            file_backed_map[std::make_pair(phys_filename, block)] =
                            process_info_map[running_process].info_bits[page];
         
            process_info_map[running_process].info_bits[page]->valid = true;
            process_info_map[running_process].info_bits[page]->resident = false;
            process_info_map[running_process].info_bits[page]->dirty = false;
            process_info_map[running_process].info_bits[page]->zero_filled = false;
            process_info_map[running_process].info_bits[page]->filename = new char[phys_filename.length()];
            strcpy(process_info_map[running_process].info_bits[page]->filename, phys_filename.c_str());
            process_info_map[running_process].info_bits[page]->block = block;

            update_read_write_bits(process_info_map[running_process].info_bits[page]);

           
        }
        
        // insert the current page table entry to the group
        process_info_map[running_process].info_bits[page]->ptable_entries.insert(
            std::make_pair(&process_info_map[running_process].page_table->ptes[page],bool(true)));
        
        // return the lowest available address of the new page        
        void* return_val = process_info_map[running_process].next_available_addr;
        process_info_map[running_process].next_available_addr =
            (char*)process_info_map[running_process].next_available_addr + VM_PAGESIZE;
            
        return return_val;

    }

}

void vm_switch(pid_t pid) {
    // switch running process id
    running_process = pid;

    // load page table for new process
    page_table_base_register = process_info_map[running_process].page_table;
};

int vm_fault(const void* addr, bool write_flag) {
    // calculate virtual page number of addr
    int page = ((char*)addr-(char*)VM_ARENA_BASEADDR)/VM_PAGESIZE;

    // check if addr is in valid part of arena
    if (addr >= process_info_map[running_process].next_available_addr || addr < VM_ARENA_BASEADDR || !process_info_map[running_process].info_bits[page]->valid )
        return -1;
    
    if (!process_info_map[running_process].info_bits[page]->resident) { // virtual page not loaded in physical memory
        int mem_page = find_physical_page(); // obtain physical memory page
        
        unsigned int buff = mem_page * VM_PAGESIZE;
        if (file_read((const char*)process_info_map[running_process].info_bits[page]->filename, 
                      process_info_map[running_process].info_bits[page]->block, 
                      (void*)&((char*)vm_physmem)[buff]) == -1) {
            // failed to read file, deallocate memory page
            used_memory[mem_page] = false;
            num_used_physmem_page--;
            return -1;
        }
        
        // update physical page number in page table
        for (auto i : process_info_map[running_process].info_bits[page]->ptable_entries){
            if(i.second) {
                i.first->ppage = mem_page;
            }
        }
        process_info_map[running_process].info_bits[page]->ppage = mem_page;

        // push virtual page info to lru clock
        lru_clock.push_back(process_info_map[running_process].info_bits[page]);
    }

    
    if (write_flag) { // write fault
        
        // for swap backed pages        
        if (process_info_map[running_process].info_bits[page]->zero_filled) { // pointing to zero-filled page
            int mem_page = find_physical_page(); // obtain physical memory page
            
            // update physical page number in page table
            for (auto i : process_info_map[running_process].info_bits[page]->ptable_entries){
                if (i.second) {
                    i.first->ppage = mem_page;
                }
            }
            process_info_map[running_process].info_bits[page]->ppage = mem_page;
            for (unsigned int i = 0; i < VM_PAGESIZE/4; ++i) 
                ((unsigned int *)vm_physmem)[(mem_page * VM_PAGESIZE)/4 + i] = 0;
            
            // push virtual page info to lru clock
            lru_clock.push_back(process_info_map[running_process].info_bits[page]);
        }
        
        // update remaining info bits
        process_info_map[running_process].info_bits[page]->dirty = true;
        process_info_map[running_process].info_bits[page]->zero_filled = false;
    }

    // update remaining info bits
    process_info_map[running_process].info_bits[page]->resident = true;
    process_info_map[running_process].info_bits[page]->referenced = true;

    update_read_write_bits(process_info_map[running_process].info_bits[page]);
    return 0;
};


void vm_destroy() {
    // loop through all virtual pages
    for (unsigned int i = 0; i < VM_ARENA_SIZE/VM_PAGESIZE; i++){
        
        if (process_info_map[running_process].info_bits[i]) { // page is valid
            auto it = process_info_map[running_process].info_bits[i]->ptable_entries.find(
                    std::make_pair(&process_info_map[running_process].page_table->ptes[i], bool(true)));
                    
            if (it != process_info_map[running_process].info_bits[i]->ptable_entries.end()) { // indicate that current process is finished with this page
                process_info_map[running_process].info_bits[i]->ptable_entries.insert(std::make_pair(it->first, bool(false)));
                process_info_map[running_process].info_bits[i]->ptable_entries.erase(it);
            }
            
            // clean up all the memory for swap-backed pages
            if (!process_info_map[running_process].info_bits[i]->filename) { // swap-backed page
                // free phs_mem that the swap-backed page is using
                if (process_info_map[running_process].info_bits[i]->resident && !process_info_map[running_process].info_bits[i]->zero_filled) { 
                    // mark physical memory as free
                    used_memory[process_info_map[running_process].page_table->ptes[i].ppage] = false;
                    num_used_physmem_page--;
                    
                    // remove page from lru_clock
                    for (auto it = lru_clock.begin(); it != lru_clock.end(); ++it) {
                        if (*it == process_info_map[running_process].info_bits[i]) {
                            lru_clock.erase(it); 
                            break;
                        }
                    }
                }

                // mark swap block as free
                used_in_swap_file[process_info_map[running_process].info_bits[i]->block] = false;
                num_used_swap_blocks--;

                // delete all the kernel page info bits for this virtual page
                delete process_info_map[running_process].info_bits[i];
                process_info_map[running_process].info_bits[i] = nullptr;
            }              
        }
    
    }
    // delete the page table for this process
    delete process_info_map[running_process].page_table;
    delete[] process_info_map[running_process].info_bits;
    process_info_map.erase(running_process);
};

