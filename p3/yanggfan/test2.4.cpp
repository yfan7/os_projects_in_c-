#include <iostream>
#include <cstring>
#include <unistd.h>
#include "vm_app.h"

using std::cout;

int main()
{
    /* Allocate swap-backed page from the arena */
    char *filename = (char *) vm_map(nullptr, 0);
    
    
    /* Write the name of the file that will be mapped */
    strcpy(filename, "shakespeare.txt");

    /* Map a page from the specified file */
    
    pid_t cpid = fork();
    if (cpid == 0) {
        char *p = (char *) vm_map (filename, 0);
        for (int i = 0; i < 3; i++){
            char *temp = (char *) vm_map(nullptr, 0);
            temp[0] = 'a';
        }
        cout << "child\n";

    }
    else {
        
        char *p = (char *) vm_map (filename, 0);
        vm_yield();
        cout << "parent\n";
    }
    /* Print the first speech from the file */
    // for (unsigned int i=0; i<2561; i++) {
	//     cout << p[i];
    // }
    exit(0);
    
}