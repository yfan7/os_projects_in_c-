#include <iostream>
#include <cstring>
#include <unistd.h>
#include "vm_app.h"

using std::cout;

int main()
{
    char *filename = (char *) vm_map(nullptr, 0);
    char *filename2 = (char *) vm_map(nullptr, 0);
    char *filename3 = (char *) vm_map(nullptr, 0);
    std::string s(4093+4096, 'a');
    s += "data3.bin";
    strcpy(filename, s.c_str());
    char *q = (char *) vm_map (filename+4093 + 4096, 0);

    for (unsigned int i=0; i<10; i++) {
        cout << q[i];
    }
    cout << "\n";

    exit(0);
    cout << filename2[0] << filename3[0];
}
