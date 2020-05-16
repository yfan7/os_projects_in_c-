#include <iostream>
#include <cstring>
#include <unistd.h>
#include "vm_app.h"

using std::cout;

int main()
{
    pid_t cpid = fork();
    if (cpid == 0) {
        vm_yield();
        cout << "child\n";

         /* Allocate swap-backed page from the arena */
        char *filename2 = (char *) vm_map(nullptr, 0);

        /* Write the name of the file that will be mapped */
        strcpy(filename2, "data2.bin");

        char *q = (char *) vm_map (filename2, 0);
    
        for(int i = 0; i < 10; ++i) {
            cout << q[i];
        }
        for (int i = 0; i < 10; i+=2) {
            q[i] = 'a';
        }
        
        cout << "\n";

    }
    else {
        cout << "parent\n";

        /* Allocate swap-backed page from the arena */
        char *filename1 = (char *) vm_map(nullptr, 0);

        /* Write the name of the file that will be mapped */
        strcpy(filename1, "data1.bin");

        char *p = (char *) vm_map (filename1, 0);
    
        for(int i = 0; i < 10; ++i) {
            cout << p[i];
        }
        cout << "\n";
    }

    exit(0);
    
}