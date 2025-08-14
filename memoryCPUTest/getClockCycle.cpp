#include <iostream>
#include <fstream>

int main() {
    std::ifstream cpu_freq("/sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq");
    unsigned long freq_khz;
    cpu_freq >> freq_khz;
    std::cout << "CPU frequency: " << freq_khz / 1000.0 << " MHz\n";
    return 0;
}
