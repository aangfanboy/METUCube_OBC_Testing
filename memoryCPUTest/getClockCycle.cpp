#include <iostream>
#include <chrono>
#include <thread>

int main() {
    auto t1 = std::chrono::high_resolution_clock::now();
    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // 0.1s delay
    auto t2 = std::chrono::high_resolution_clock::now();

    auto delta_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();
    double freq_mhz = 1e3 / static_cast<double>(delta_ns) * 1e6; // Not real CPU MHz, just illustrative

    std::cout << "Delta time: " << delta_ns << " ns\n";
    std::cout << "Approx. frequency: " << freq_mhz << " MHz\n";

    return 0;
}
