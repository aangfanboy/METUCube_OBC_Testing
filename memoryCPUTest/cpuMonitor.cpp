#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <chrono>
#include <thread>
#include <csignal>
#include <vector>
#include <filesystem>

bool running = true;

void handle_interrupt(int) {
    running = false;
}

// Read CPU temperature from any available thermal zone
double read_temperature() {
    for (const auto& entry : std::filesystem::directory_iterator("/sys/class/thermal")) {
        if (entry.path().filename().string().find("thermal_zone") != std::string::npos) {
            std::ifstream type_file(entry.path() / "type");
            std::string type;
            if (type_file >> type && type.find("cpu") != std::string::npos) {
                std::ifstream temp_file(entry.path() / "temp");
                int temp_milli;
                if (temp_file >> temp_milli) {
                    return temp_milli / 1000.0;
                }
            }
        }
    }
    return -1.0;  // not found
}

// Read average CPU usage by comparing idle/delta
double read_cpu_usage() {
    static long prev_idle = 0, prev_total = 0;
    std::ifstream file("/proc/stat");
    std::string line;
    getline(file, line);
    std::istringstream ss(line);

    std::string cpu;
    long user, nice, system, idle, iowait, irq, softirq, steal;
    ss >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal;
    long idle_time = idle + iowait;
    long total_time = user + nice + system + idle_time + irq + softirq + steal;

    long delta_total = total_time - prev_total;
    long delta_idle = idle_time - prev_idle;

    prev_total = total_time;
    prev_idle = idle_time;

    if (delta_total == 0) return 0.0;
    return 100.0 * (1.0 - static_cast<double>(delta_idle) / delta_total);
}

// Read average CPU frequency from cpufreq sysfs
int read_cpu_frequency() {
    std::vector<std::string> cpu_paths;
    for (const auto& entry : std::filesystem::directory_iterator("/sys/devices/system/cpu")) {
        if (entry.path().filename().string().find("cpu") == 0 && entry.is_directory()) {
            cpu_paths.push_back(entry.path().string());
        }
    }

    int freq_sum = 0;
    int count = 0;
    for (const auto& cpu_path : cpu_paths) {
        std::ifstream freq_file(cpu_path + "/cpufreq/scaling_cur_freq");
        int freq_khz;
        if (freq_file >> freq_khz) {
            freq_sum += freq_khz;
            count++;
        }
    }

    if (count > 0) return freq_sum / count / 1000; // Convert to MHz
    return -1;
}

int main(int argc, char* argv[]) {
    signal(SIGINT, handle_interrupt);
    int period_ms = 1000;
    if (argc > 1) period_ms = std::stoi(argv[1]);

    read_cpu_usage();  // Prime CPU delta

    std::this_thread::sleep_for(std::chrono::milliseconds(period_ms));

    while (running) {
        time_t now = time(nullptr);

        double temp = read_temperature();
        int freq = read_cpu_frequency();
        double usage = read_cpu_usage();

        std::cout << "[" << now << "] ";
        if (temp >= 0)
            std::cout << "Temp: " << temp << "°C | ";
        if (freq >= 0)
            std::cout << "Freq: " << freq << " MHz | ";
        std::cout << "CPU: " << usage << "%\n";

        std::this_thread::sleep_for(std::chrono::milliseconds(period_ms));
    }

    return 0;
}
