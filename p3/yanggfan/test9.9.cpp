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
    char *filename3 = (char *) vm_map(nullptr, 0);

    /* Write the name of the file that will be mapped */
    strcpy(filename1, "data1.bin");

    strcpy(filename2, "data2.bin");

    strcpy(filename3, "data3.bin");

    /* Map a page from the specified file */
    char *p1 = (char *) vm_map (filename1, 1);
    char *p2 = (char *) vm_map (filename1, 3);

    char *q1 = (char *) vm_map (filename2, 1);
    char *q2 = (char *) vm_map (filename2, 3);

    char *r1 = (char *) vm_map (filename3, 1);
    char *r2 = (char *) vm_map (filename3, 3);

    cout << "p1 \n";
    for (unsigned int i=0; i<10; i++) {
	    cout << p1[i];
    }
    cout << "p2 \n";
    for (unsigned int i=0; i<10; i++) {
	    cout << p2[i];
    }
    cout << " q1 \n";

    for (unsigned int i=0; i<10; i++) {
	    cout << q1[i];
    }
    cout << "q2\n";

    for (unsigned int i=0; i<10; i++) {
	cout << q2[i];
    }
    cout << "r1\n";

    for (unsigned int i=0; i<10; i++) {
	    cout << r1[i];
    }
    cout << "r2\n";

    for (unsigned int i=0; i<10; i++) {
	    cout << r2[i];
    }
    cout << "\n";



    for (unsigned int i=0; i<10; i++) {
	    p1[i] = 65 + i;
    }    
    for (unsigned int i=0; i<10; i++) {
	    p2[i] = 65 + i;
    }

    for (unsigned int i=0; i<10; i++) {
	    q1[i] = 65 + i;
    }
    for (unsigned int i=0; i<10; i++) {
	    q2[i] = 65 + i;
    }

    for (unsigned int i=0; i<10; i++) {
	    r1[i] = 65 + i;
    }
    for (unsigned int i=0; i<10; i++) {
	    r2[i] = 65 + i;
    }

    char *filename = (char *) vm_map(nullptr, 0);
    strcpy(filename, "data1.bin");

    char *file = (char *) vm_map (filename, 1);
    for (unsigned int i=0; i<10; i++) {
	    cout << file[i];
    }
    cout << "\n";

    file = (char *) vm_map (filename, 3);
    for (unsigned int i=0; i<10; i++) {
	    cout << file[i];
    }
    cout << "\n";

    strcpy(filename, "data2.bin");

    file = (char *) vm_map (filename, 1);
    for (unsigned int i=0; i<10; i++) {
	    cout << file[i];
    }
    cout << "\n";

    file = (char *) vm_map (filename, 3);
    for (unsigned int i=0; i<10; i++) {
	    cout << file[i];
    }
    cout << "\n";

    strcpy(filename, "data3.bin");

    file = (char *) vm_map (filename, 1);
    for (unsigned int i=0; i<10; i++) {
	    cout << file[i];
    }
    cout << "\n";

    file = (char *) vm_map (filename, 3);
    for (unsigned int i=0; i<10; i++) {
	    cout << file[i];
    }
    cout << "\n";

    exit(0);
    
}