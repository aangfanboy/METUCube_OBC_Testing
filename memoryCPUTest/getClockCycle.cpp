#include <iostream>
#include <chrono>
#include <thread>
#include <fstream>

int main() {
    auto t1 = std::chrono::high_resolution_clock::now();
    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // 0.1s delay
    auto t2 = std::chrono::high_resolution_clock::now();

    auto delta_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();
    double freq_mhz = 1e3 / static_cast<double>(delta_ns) * 1e6; // Not real CPU MHz, just illustrative

    std::cout << "Delta time: " << delta_ns << " ns\n";
    std::cout << "Approx. frequency: " << freq_mhz << " MHz\n";
    
    std::ifstream cpu_freq("/sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq");
    unsigned long freq_khz;
    cpu_freq >> freq_khz;

    std::cout << "CPU frequency: " << freq_khz / 1000.0 << " MHz\n";

    return 0;
}
