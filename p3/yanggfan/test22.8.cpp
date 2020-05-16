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
    char *ptr1 = (char *) vm_map (filename1, 0);
    char *ptr2 = (char *) vm_map (filename1, 1);
    char *ptr3 = (char *) vm_map (filename1, 2);
    char *ptr4 = (char *) vm_map (filename1, 3);
    char *ptr5 = (char *) vm_map (filename2, 0);
    char *ptr6 = (char *) vm_map (filename2, 1);
    char *ptr7 = (char *) vm_map (filename2, 2);

    for(int i = 0; i < 10; ++i) {
        cout << ptr1[i];
    }
    cout << "\n";
    for(int i = 0; i < 10; ++i) {
        cout << ptr2[i];
    }
    cout << "\n";
    for(int i = 0; i < 10; ++i) {
        cout << ptr3[i];
    }
    cout << "\n";
    for(int i = 0; i < 10; ++i) {
        cout << ptr4[i];
    }
    cout << "\n";
    for(int i = 0; i < 10; ++i) {
        cout << ptr5[i];
    }
    cout << "\n";
    for(int i = 0; i < 10; ++i) {
        cout << ptr6[i];
    }
    cout << "\n";
    for(int i = 0; i < 10; ++i) {
        cout << ptr7[i];
    }
    cout << "\n";


    pid_t cpid = fork();
    if (cpid == 0) {
        vm_yield();
        cout << "child\n";

        for(int i = 0; i < 10; ++i) {
            cout << ptr1[i];
        }
        cout << "\n";
        for(int i = 0; i < 10; ++i) {
            cout << ptr2[i];
        }
        cout << "\n";
        for(int i = 0; i < 10; ++i) {
            cout << ptr3[i];
        }
        cout << "\n";
        for(int i = 0; i < 10; ++i) {
            cout << ptr4[i];
        }
        cout << "\n";
        for(int i = 0; i < 10; ++i) {
            cout << ptr5[i];
        }
        cout << "\n";
        for(int i = 0; i < 10; ++i) {
            cout << ptr6[i];
        }
        cout << "\n";
        for(int i = 0; i < 10; ++i) {
            cout << ptr7[i];
        }
        cout << "\n";

        ptr1[0] = 'a';
        ptr2[0] = 'a';
        ptr3[0] = 'a';
        ptr4[0] = 'a';
        ptr5[0] = 'a';
        ptr6[0] = 'a';
        ptr7[0] = 'a';

        vm_yield();

        for(int i = 0; i < 10; ++i) {
            cout << ptr1[i];
        }
        cout << "\n";
        for(int i = 0; i < 10; ++i) {
            cout << ptr2[i];
        }
        cout << "\n";
        for(int i = 0; i < 10; ++i) {
            cout << ptr3[i];
        }
        cout << "\n";
        for(int i = 0; i < 10; ++i) {
            cout << ptr4[i];
        }
        cout << "\n";
        for(int i = 0; i < 10; ++i) {
            cout << ptr5[i];
        }
        cout << "\n";
        for(int i = 0; i < 10; ++i) {
            cout << ptr6[i];
        }
        cout << "\n";
        for(int i = 0; i < 10; ++i) {
            cout << ptr7[i];
        }
        cout << "\n";
    }
    else {
        cout << "parent\n";

        for(int i = 0; i < 10; ++i) {
            cout << ptr1[i];
        }
        cout << "\n";
        for(int i = 0; i < 10; ++i) {
            cout << ptr2[i];
        }
        cout << "\n";
        for(int i = 0; i < 10; ++i) {
            cout << ptr3[i];
        }
        cout << "\n";
        for(int i = 0; i < 10; ++i) {
            cout << ptr4[i];
        }
        cout << "\n";
        for(int i = 0; i < 10; ++i) {
            cout << ptr5[i];
        }
        cout << "\n";
        for(int i = 0; i < 10; ++i) {
            cout << ptr6[i];
        }
        cout << "\n";
        for(int i = 0; i < 10; ++i) {
            cout << ptr7[i];
        }
        cout << "\n";

        vm_yield();

        for(int i = 0; i < 10; ++i) {
            cout << ptr1[i];
        }
        cout << "\n";
        for(int i = 0; i < 10; ++i) {
            cout << ptr2[i];
        }
        cout << "\n";
        for(int i = 0; i < 10; ++i) {
            cout << ptr3[i];
        }
        cout << "\n";
        for(int i = 0; i < 10; ++i) {
            cout << ptr4[i];
        }
        cout << "\n";
        for(int i = 0; i < 10; ++i) {
            cout << ptr5[i];
        }
        cout << "\n";
        for(int i = 0; i < 10; ++i) {
            cout << ptr6[i];
        }
        cout << "\n";
        for(int i = 0; i < 10; ++i) {
            cout << ptr7[i];
        }
        cout << "\n";
    }
    /* Print the first speech from the file */
    // for (unsigned int i=0; i<2561; i++) {
	//     cout << p[i];
    // }
    exit(0);
    
}