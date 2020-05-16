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
    /* Map a page from the specified file */
    char ** ptr =(char **) (void *)vm_map(nullptr, 0);
    vm_yield();
    /* Print the first speech from the file */
    for (unsigned int i=0; i<2; i++) {
        ptr[i] = (char *) vm_map(nullptr, 0);
    }


    ptr[0] = (char*)vm_map(filename, 0);
    // char temp[6] = "value";
    // ptr[0] = temp;
    ptr[1] = (char*)vm_map(ptr[0], 1);
    cout << "30\n";
    // char temp2[8] = "writing";
    // ptr[1] = temp2;
    strcpy(ptr[1], "writing");

    exit(0);
    
    // exit(0);
}
