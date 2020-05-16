#include <iostream>
#include <cstring>
#include <unistd.h>
#include "vm_app.h"

using std::cout;

int main()
{
    fork();

    char *filename = (char *) vm_map(nullptr, 0);
    char *filename2 = (char *) vm_map(nullptr, 0);
    /* Write the name of the file that will be mapped */
    strcpy(filename, "data1.bin");
    strcpy(filename2, "data2.bin");
    /* Map a page from the specified file */
    char ** ptr =(char **) (void *)vm_map(nullptr, 0);
    vm_yield();
    /* Print the first speech from the file */
    for (unsigned int i=0; i<15; i++) {
        ptr[i] = (char *) vm_map(nullptr, 0);
        // if(!ptr[i]) {
        //     printf("nullptr at i = %i", i);
        //     exit(0);
        // }
    }
    // exit(0);
    // for (unsigned int i=0; i<100; i++) {
    //     // cout << ptr[0];
         
    // }
    // vm_yield();
    // strcpy(ptr[0], "some string");
    // cout << ptr[0];
    // // strcpy(ptr[200], "200 this\n ");
    // strcpy(ptr[45], "45 this\n ");
    
    // vm_yield();

    ptr[0] = (char*)vm_map(filename, 0);
    // char temp[6] = "value";
    // ptr[0] = temp;
    strcpy(ptr[0], "value");
    ptr[1] = (char*)vm_map(filename, 1);
    // char temp2[8] = "writing";
    // ptr[1] = temp2;
    strcpy(ptr[1], "writing");

    exit(0);

    for (unsigned int i=0; i<1024; i++) {
        ptr[i] = (char*)vm_map(filename, 0);
        // printf("%s", ptr[i]) ;
    }

    for (unsigned int i=0; i<1024; i++) {
        ptr[i] = (char*)vm_map(filename2, 0);
        // printf("%s", ptr[i]) ;
    }
    printf("%s", (char*)vm_map(filename2, 4)) ;
    
    // exit(0);
}
