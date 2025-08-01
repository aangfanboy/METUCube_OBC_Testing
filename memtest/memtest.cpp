#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>
#include <cstring>
#include <unistd.h>  // For usleep

using namespace std;

#define BLOCK_SIZE_MB 3000  // Size of block to test (in MB)
#define PATTERN_COUNT 4   // Number of patterns

uint8_t patterns[PATTERN_COUNT] = {0xAA, 0x55, 0xFF, 0x00};

void corrupt_memory(uint8_t* buffer, size_t size, size_t byte_index, uint8_t mask);


bool test_memory_block(uint8_t* mem, size_t size, ofstream& logfile) {
    bool all_passed = true;

    // first write to all memory locations
    for (size_t i = 0; i < size; ++i) {
        mem[i] = patterns[i % PATTERN_COUNT];
    }

    // simulate a flip in memory
    size_t corrupt_index = size / 2 + 2;  // Corrupt the middle byte
    uint8_t corrupt_mask = 0x01;  // Flip the least significant bit
    corrupt_memory(mem, size, corrupt_index, corrupt_mask);

    // verify the written patterns
    for (size_t i = 0; i < size; ++i) {
        if (mem[i] != patterns[i % PATTERN_COUNT]) {
            logfile << "Memory test failed at index " << i << ": expected "
                    << (int)patterns[i % PATTERN_COUNT] << ", got "
                    << (int)mem[i] << endl;
            all_passed = false;
        }
    }

    return all_passed;
}

void corrupt_memory(uint8_t* buffer, size_t size, size_t byte_index, uint8_t mask) {
    if (byte_index < size) {
        buffer[byte_index] ^= mask;  // Flip bits
    }
}

int main() {
    size_t block_size = static_cast<size_t>(BLOCK_SIZE_MB) * 1024 * 1024;  // MB to Bytes
    ofstream logfile("memtest_log.txt");

    cout << "Allocating " << BLOCK_SIZE_MB << "MB of memory..." << endl;
    uint8_t* memory = new (nothrow) uint8_t[block_size];

    if (!memory) {
        cerr << "Memory allocation failed!" << endl;
        return 1;
    }

    memset(memory, 0, block_size);  // Initialize memory to zero

    cout << "Memory allocated at address: " << (void*)memory << endl;
    cout << "Starting memory test..." << endl;

    auto start = chrono::high_resolution_clock::now();
    bool result = test_memory_block(memory, block_size, logfile);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;

    if (result) {
        cout << "Memory test passed in " << duration.count() << " seconds." << endl;
    } else {
        cout << "Memory test failed. Check memtest_log.txt for details." << endl;
    }

    delete[] memory;
    logfile.close();
    return 0;
}
