#include <iostream>
#include <chrono>

int main() {
    auto now = std::chrono::high_resolution_clock::now();
    auto ns  = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count();

    double freq_mhz = 1e3 / static_cast<double>(ns) * 1e6; // Not real CPU MHz, just illustrative

    std::cout << "Time since epoch: " << ns << " ns\n";
    std::cout << "Estimated CPU frequency: " << freq_mhz << " MHz\n";
    return 0;
}
