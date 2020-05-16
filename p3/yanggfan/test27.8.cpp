#include <iostream>
#include <cstring>
#include <unistd.h>
#include "vm_app.h"

using std::cout;

int main()
{
    pid_t cpid = fork();
    if (cpid == 0) {
        /* Allocate swap-backed page from the arena */
        char *filename = (char *) vm_map(nullptr, 0);

        /* Write the name of the file that will be mapped */
        strcpy(filename, "data1.bin");

        /* Map a page from the specified file */
        char *p = (char *) vm_map (filename, 0);
        char *q = (char *) vm_map (filename, 0);


        for (unsigned int i=0; i<10; i++) {
            cout << p[i];
        }
        cout << "\n";
        for (unsigned int i=0; i<10; i++) {
            cout << q[i];
        }
        cout << "\n";
        for (unsigned int i=0; i<10; i+=4) {
            p[i] = 'b';
        }
        cout << "\n";
        for (unsigned int i=0; i<10; i++) {
            cout << p[i];
        }
        cout << "\n";
        for (unsigned int i=0; i<10; i++) {
            cout << q[i];
        }
    }
    else {

        vm_yield();
        for(int i = 0; i < 4; ++i) {
        /* Allocate swap-backed page from the arena */
        char *filename = (char *) vm_map(nullptr, 0);

        /* Write the name of the file that will be mapped */
        strcpy(filename, "shakespeare.txt");
        }

        pid_t cpid = fork();
        if (cpid == 0) {
            vm_yield();
            /* Allocate swap-backed page from the arena */
            char *filename = (char *) vm_map(nullptr, 0);

            /* Write the name of the file that will be mapped */
            strcpy(filename, "data1.bin");

            /* Map a page from the specified file */
            char *p = (char *) vm_map (filename, 0);
            char *q = (char *) vm_map (filename, 0);


            for (unsigned int i=0; i<10; i++) {
                cout << p[i];
            }
            cout << "\n";
            for (unsigned int i=0; i<10; i++) {
                cout << q[i];
            }
            cout << "\n";
            for (unsigned int i=0; i<10; i+=4) {
                p[i] = 'b';
            }
            cout << "\n";
            for (unsigned int i=0; i<10; i++) {
                cout << p[i];
            }
            cout << "\n";
            for (unsigned int i=0; i<10; i++) {
                cout << q[i];
            }
        }
        
    }
    
}