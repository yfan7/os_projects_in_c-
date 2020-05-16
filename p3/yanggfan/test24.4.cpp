#include <iostream>
#include <cstring>
#include <string>
#include <unistd.h>
#include "vm_app.h"

using std::cout;

int main()
{

    /* Allocate swap-backed page from the arena */
    char *filename = (char *) vm_map(nullptr, 0);
    strcpy(filename, "data1.bin");

    /* Map a page from the specified file */
    char *p = (char *) vm_map (filename, 0);
    p[0] = 'A';
    
    char* s1 = (char *) vm_map(nullptr, 0);
    strcpy(s1, "nothing");
    char* s2 = (char *) vm_map(nullptr, 0);
    strcpy(s2, "nothing");
    char* s3 = (char *) vm_map(nullptr, 0);
    strcpy(s3, "nothing");



    pid_t cpid = fork();
    if (cpid == 0) {
        vm_yield();
        cout << "child\n";
        for(int i = 0; i < 10; ++i) {
            p[i] = 65 + i;
        }

    }
    else {
        cout << "parent\n";
        strcpy(filename, "data1.bin");

    }
    /* Print the first speech from the file */
    // for (unsigned int i=0; i<2561; i++) {
	//     cout << p[i];
    // }
    exit(0);
    
}
