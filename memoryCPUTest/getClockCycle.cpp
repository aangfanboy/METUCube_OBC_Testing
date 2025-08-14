#include <iostream>
#include <cstdint>

inline uint64_t read_cntvct() {
    uint64_t val;
    asm volatile("mrs %0, cntvct_el0" : "=r"(val)); // Read virtual counter
    return val;
}

inline uint64_t read_cntfrq() {
    uint64_t val;
    asm volatile("mrs %0, cntfrq_el0" : "=r"(val)); // Read counter frequency
    return val;
}

int main() {
    uint64_t freq = read_cntfrq();   // Counts per second
    uint64_t cycles = read_cntvct(); // Raw cycle count
    double ns = (static_cast<double>(cycles) / freq) * 1e9; // Convert to nanoseconds

    std::cout << "Raw cycle count : " << cycles << "\n";
    std::cout << "Counter frequency: " << freq << " Hz\n";
    std::cout << "Time since boot  : " << ns << " ns\n";
    
    return 0;
}
