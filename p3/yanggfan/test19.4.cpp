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

}

