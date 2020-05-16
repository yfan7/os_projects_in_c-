#include <iostream>
#include <cstring>
#include <unistd.h>
#include "vm_app.h"

using std::cout;

int main()
{

    pid_t cpid = fork();
    if (cpid == 0) {
        cout << "child\n";

         /* Allocate swap-backed page from the arena */
        char *filename1 = (char *) vm_map(nullptr, 0);

        /* Write the name of the file that will be mapped */
        strcpy(filename1, "stuff.bin");

        char *q = (char *) vm_map (filename1, 0);
 
        q[0] = 'A';
        

    }
    else {
        vm_yield();
        cout << "parent\n";

        for(int i = 0; i < 20; ++i) {
            char *filename1 = (char *) vm_map(nullptr, 0);
            strcpy(filename1, "data1.bin");
        }

    }
    exit(0);
    
}