#include <iostream>
#include <cstring>
#include <unistd.h>
#include "vm_app.h"

using std::cout;

int main()
{
    // fork();

    char *filename = (char *) vm_map(nullptr, 0);
    char *filename2 = (char *) vm_map(nullptr, 0);
    /* Write the name of the file that will be mapped */
    strcpy(filename, "data1.bin");
    strcpy(filename2, "data2.bin");

    char* ptr = (char*)vm_map(filename, 0);
    strcpy(ptr, "value");

    
    char *filename3 = (char *) vm_map(nullptr, 0);
    strcpy(filename3, "data1.bin");


    char* ptr2 = (char*)vm_map(filename, 0);
    strcpy(ptr2, "value");
    char *filename4 = (char *) vm_map(nullptr, 0);
    strcpy(filename4, "data1.bin");
    char *filename5 = (char *) vm_map(nullptr, 0);
    strcpy(filename5, "data1.bin");
    char *filename6 = (char *) vm_map(nullptr, 0);
    strcpy(filename6, "data1.bin");

    char* ptr3 = (char*)vm_map(filename, 0);
    strcpy(ptr3, "value");

    
    char *filename7 = (char *) vm_map(nullptr, 0);
    /* Write the name of the file that will be mapped */
    strcpy(filename7, "data1.bin");

    char* ptr4 = (char*)vm_map(filename7, 0);
    char* ptr5 = (char*)vm_map(filename7, 0);
    char* ptr6 = (char*)vm_map(filename7, 0);
    char* ptr7 = (char*)vm_map(filename7, 0);
    strcpy(ptr7, "value");
    strcpy(ptr7, ptr4);
    strcpy(ptr4, "something");

    pid_t cpid = fork();

    if (cpid == 0) {
        cout << "child\n";
        char *filename = (char *) vm_map(nullptr, 0);
        char *filename2 = (char *) vm_map(nullptr, 0);
        /* Write the name of the file that will be mapped */
        strcpy(filename, "data1.bin");
        strcpy(filename2, "data2.bin");

        char* ptr = (char*)vm_map(filename, 0);
        strcpy(ptr, "value");

        
        char *filename3 = (char *) vm_map(nullptr, 0);
        strcpy(filename3, "data1.bin");


        char* ptr2 = (char*)vm_map(filename, 0);
        strcpy(ptr2, "value");




        char *filename4 = (char *) vm_map(nullptr, 0);
        strcpy(filename4, "data1.bin");
        char *filename5 = (char *) vm_map(nullptr, 0);
        strcpy(filename5, "data1.bin");
        char *filename6 = (char *) vm_map(nullptr, 0);
        strcpy(filename6, "data1.bin");

        char* ptr3 = (char*)vm_map(filename, 0);
        strcpy(ptr3, "value");

        
        char *filename7 = (char *) vm_map(nullptr, 0);
        /* Write the name of the file that will be mapped */
        strcpy(filename7, "data1.bin");

        char* ptr4 = (char*)vm_map(filename7, 0);
        char* ptr5 = (char*)vm_map(filename7, 0);
        char* ptr6 = (char*)vm_map(filename7, 0);
        char* ptr7 = (char*)vm_map(filename7, 0);
        strcpy(ptr7, "value");
        strcpy(ptr7, ptr4);
        strcpy(ptr4, "something");

    }
    else {
        vm_yield();

        cout << "parent\n";

        /* Allocate swap-backed page from the arena */
        char *filename1 = (char *) vm_map(nullptr, 0);

        /* Write the name of the file that will be mapped */
        strcpy(filename1, "data1.bin");

        char *p = (char *) vm_map (filename1, 0);
    
        for (int i = 0; i < 10; ++i) {
            cout << p[i];
        }
        for (int i = 0; i < 10; ++i) {
            p[i] = i;
        }
        cout << "\n";
    }


    exit(0);
}
