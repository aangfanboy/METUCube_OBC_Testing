#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <sstream>
#include <chrono>
#include <thread>
#include <csignal>
#include <vector>

bool running = true;

void handle_interrupt(int) {
    running = false;
}

std::unordered_map<std::string, long> read_meminfo() {
    std::ifstream file("/proc/meminfo");
    std::string key;
    long value;
    std::string unit;
    std::unordered_map<std::string, long> meminfo;

    while (file >> key >> value >> unit) {
        // Remove the colon from the key
        key.pop_back();
        meminfo[key] = value;  // value is in kB
    }

    return meminfo;
}

int main() {
    signal(SIGINT, handle_interrupt);

    while (running)
    {
        auto meminfo = read_meminfo();

        long mem_total = meminfo["MemTotal"];
        long mem_free = meminfo["MemFree"];
        long buffers = meminfo["Buffers"];
        long cached = meminfo["Cached"];
        long sreclaimable = meminfo["SReclaimable"];
        long shmem = meminfo["Shmem"];
    
        long mem_used = mem_total - mem_free - buffers - cached - sreclaimable + shmem;
    
        std::cout << "Total Memory: " << mem_total / 1024 << " MB" << std::endl;
        std::cout << "Used Memory : " << mem_used / 1024 << " MB" << std::endl;
        std::cout << "Free Memory : " << mem_free / 1024 << " MB" << std::endl;
        std::cout << "Buffers    : " << buffers / 1024 << " MB" << std::endl;
        std::cout << "Cached     : " << cached / 1024 << " MB" << std::endl;
        std::cout << "SReclaimable: " << sreclaimable / 1024 << " MB" << std::endl;
        std::cout << "Shmem      : " << shmem / 1024 << " MB" << std::endl;

        std::cout << "----------------------------------------" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1)); // Sleep for 1 second
    }

    return 0;
}
