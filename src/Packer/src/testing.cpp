#include <iostream>
#include <vector>
#include <bits/stdc++.h>
using namespace std;

int testing()
{
    printf("Hello, World!");
    return 0;

    ifstream file("myfile", ios::binary | ios::ate);
    streamsize size = file.tellg();
    file.seekg(0, ios::beg);

    vector<char> buffer(size);
    if (file.read(buffer.data(), size))
    {
        /* worked! */
    }
}
