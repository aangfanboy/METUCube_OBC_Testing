#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>
#include <cstring>

using namespace std;
int main() {
    uint8_t* p = new uint8_t[1];
    cout << "Memory address: " << (void*)p << endl;

    if (p) {
        cout << "Memory allocated successfully: " << (int)*p << endl;
        delete[] p;  // Free the allocated memory
    } else {
        cerr << "Memory allocation failed!" << endl;
        return 1;
    }

    return 0;
}
