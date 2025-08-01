#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>

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
    auto meminfo = read_meminfo();

    long mem_total = meminfo["MemTotal"];
    long mem_free = meminfo["MemFree"];
    long buffers = meminfo["Buffers"];
    long cached = meminfo["Cached"];
    long sreclaimable = meminfo["SReclaimable"];
    long shmem = meminfo["Shmem"];

    // Calculate used memory: total - free - buffers - cached (+shared reclaimable - shmem)
    long mem_used = mem_total - mem_free - buffers - cached - sreclaimable + shmem;

    std::cout << "Total Memory: " << mem_total / 1024 << " MB" << std::endl;
    std::cout << "Used Memory : " << mem_used / 1024 << " MB" << std::endl;
    std::cout << "Free Memory : " << mem_free / 1024 << " MB" << std::endl;
    std::cout << "Buffers    : " << buffers / 1024 << " MB" << std::endl;
    std::cout << "Cached     : " << cached / 1024 << " MB" << std::endl;
    std::cout << "SReclaimable: " << sreclaimable / 1024 << " MB" << std::endl;
    std::cout << "Shmem      : " << shmem / 1024 << " MB" << std::endl;

    return 0;
}
