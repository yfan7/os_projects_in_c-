#include <iostream>
#include <cstring>
#include <unistd.h>
#include "vm_app.h"

using std::cout;

int main()
{
    char *p = (char *) vm_map(nullptr, 0);
    char *q = (char *) vm_map(nullptr, 0);
    char *r = (char *) vm_map(nullptr, 0);
    char *s = (char *) vm_map(nullptr, 0);

    strcpy(p, "p1");
    cout << p << "\n";
    
    strcpy(q, "q1");
    cout << q << "\n";

    strcpy(r, "r1");
    cout << r << "\n";

    strcpy(s, "s1");
    cout << s << "\n";

    strcpy(p, "p2");
    cout << p << "\n";

    strcpy(q, "q2");
    cout << q << "\n";

    char *filename1 = (char *) vm_map(nullptr, 0);
    char *filename2 = (char *) vm_map(nullptr, 0);

    /* Write the name of the file that will be mapped */
    strcpy(filename1, "data1.bin");
    strcpy(filename2, "data2.bin");
    
    char *pp = (char *) vm_map (filename1, 0);
    char *rr = (char *) vm_map (filename1, 2);
    char *ss = (char *) vm_map (filename1, 2);

    char *qq = (char *) vm_map (filename2, 0);
    char *tt = (char *) vm_map (filename2, 3);
    char *uu = (char *) vm_map (filename2, 3);
    

    for (int i = 0; i < 10; ++i) {
        cout << pp[i];
    }
    cout << "\n";

    for (int i = 0; i < 10; ++i) {
        cout << qq[i];
    }
    cout << "\n";

    for (int i = 0; i < 10; i+=2) {
        pp[i] = 'a';
        rr[i] = 'A';
        ss[i] = '1';
    }


    pid_t cpid = fork();

    if (cpid == 0) {
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

    exit(0);

    cout << tt[0] << uu[0];
    
}