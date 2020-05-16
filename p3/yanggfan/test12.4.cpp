#include <iostream>
#include <cstring>
#include <unistd.h>
#include "vm_app.h"

using std::cout;

int main()
{

    char *filename = (char *) vm_map(nullptr, 0);
    /* Write the name of the file that will be mapped */
    strcpy(filename, "data1.bin");
    /* Print the first speech from the file */
    char ** ptr =(char **) (void *)vm_map(nullptr, 0);
    ptr[0] = (char*)vm_map(filename, 0);
    // char temp[6] = "value";
    // ptr[0] = temp;
    strcpy(ptr[0], "value");
    char *filename1 = (char *) vm_map(nullptr, 0);

    strcpy(filename1, "dfsdf");
    char * bad_filename = (char *) vm_map(filename1, 0);
    for(int i = 0; i < 4096; ++i) {
        char *filename2 = (char *) vm_map(bad_filename, i);
    }

    exit(0);

}
