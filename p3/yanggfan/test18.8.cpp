#include <iostream>
#include <cstring>
#include <unistd.h>
#include "vm_app.h"

using std::cout;

int main()
{
    pid_t cpid = fork();
    if (cpid == 0) {
        vm_yield();
        cout << "child\n";

        /* Allocate swap-backed page from the arena */
        char *filename1 = (char *) vm_map(nullptr, 0);
        char *filename2 = (char *) vm_map(nullptr, 0);

        /* Write the name of the file that will be mapped */
        strcpy(filename1, "data1.bin");
        strcpy(filename2, "data2.bin");
        
        char *p = (char *) vm_map (filename1, 0);
        char *r = (char *) vm_map (filename1, 2);
        char *s = (char *) vm_map (filename1, 2);

        char *q = (char *) vm_map (filename2, 0);
        char *t = (char *) vm_map (filename2, 3);
        char *u = (char *) vm_map (filename2, 3);
        
    
        for (int i = 0; i < 10; ++i) {
            cout << p[i];
        }
        cout << "\n";

        for (int i = 0; i < 10; ++i) {
            cout << q[i];
        }
        cout << "\n";

        for (int i = 0; i < 10; i+=2) {
            p[i] = 'a';
            r[i] = 'A';
            s[i] = '1';
        }

        for (int i = 0; i < 10; i+=4) {
            q[i] = 'b';
            t[i] = 'B';
            u[i] = '2';
        }

        for (int i = 0; i < 10; ++i) {
            cout << p[i];
        }
        cout << "\n";
        for (int i = 0; i < 10; ++i) {
            cout << r[i];
        }
        cout << "\n";
        for (int i = 0; i < 10; ++i) {
            cout << s[i];
        }
        cout << "\n";
        for (int i = 0; i < 10; ++i) {
            cout << q[i];
        }
        cout << "\n";
        for (int i = 0; i < 10; ++i) {
            cout << t[i];
        }
        cout << "\n";
        for (int i = 0; i < 10; ++i) {
            cout << u[i];
        }
        cout << "\n";

    }
    else {
        cout << "parent\n";

        /* Allocate swap-backed page from the arena */
        char *filename1 = (char *) vm_map(nullptr, 0);

        /* Write the name of the file that will be mapped */
        strcpy(filename1, "data1.bin");

        char *p = (char *) vm_map (filename1, 0);
    
        for (int i = 0; i < 10; ++i) {
            cout << p[i];
        }
        cout << "\n";
    }

    exit(0);
    
}