#include <iostream>
#include <cstring>
#include <unistd.h>
#include "vm_app.h"

using std::cout;

int main()
{
    char *filename = (char *) vm_map(nullptr, 0);

    strcpy(filename, "dfsdf");
    char* bad_filename = (char *) vm_map(filename, 0);
    char* filename2 = nullptr;
    for(int i = 0; i < 4096; ++i) {
        filename2 = (char *) vm_map(bad_filename, i);
    }

    exit(0);
    cout << filename2[0];
}
