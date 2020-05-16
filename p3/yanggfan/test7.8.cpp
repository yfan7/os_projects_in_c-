#include <iostream>
#include <cstring>
#include <unistd.h>
#include "vm_app.h"

using std::cout;

int main()
{
    /* Allocate swap-backed page from the arena */
    char *filename1 = (char *) vm_map(nullptr, 0);
    char *filename2 = (char *) vm_map(nullptr, 0);

    /* Write the name of the file that will be mapped */
    strcpy(filename1, "data1.bin");
    strcpy(filename2, "data1.bin");

    /* Map a page from the specified file */
    char *p = (char *) vm_map (filename1, 0);
    char *q = (char *) vm_map (filename2, 0);
    for(int i = 0; i < 100; ++i) {
        cout << p[i];
    }
    cout << "\n";

    for(int i = 0; i < 100; ++i) {
        cout << q[i];
    }
    cout << "\n";
    // pid_t cpid = fork();
    // if (cpid == 0) {
        cout << "child\n";
        for(int i = 0; i < 100; ++i) {
            cout << p[i];
        }
        cout << "\n";

        for(int i = 0; i < 100; ++i) {
            cout << q[i];
        }
        cout << "\n";

        for(int i = 0; i < 100; ++i) {
            p[i] = 1;
            q[i] = 1;
        }
        

    // }
    // else {
    //     vm_yield();
    //     cout << "parent\n";

    //     for(int i = 0; i < 100; ++i) {
    //         cout << p[i];
    //     }
    //     cout << "\n";

    //     for(int i = 0; i < 100; ++i) {
    //         cout << q[i];
    //     }
    //     cout << "\n";
    // }
    /* Print the first speech from the file */
    // for (unsigned int i=0; i<2561; i++) {
	//     cout << p[i];
    // }
    exit(0);
    
}