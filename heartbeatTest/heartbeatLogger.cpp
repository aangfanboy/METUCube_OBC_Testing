#include <iostream>
#include <fstream>
#include <ctime>
#include <unistd.h>
#include <csignal>

const char* LOG_FILE_PATH = "heartbeat_log_unix.txt";
bool running = true;

// Get current UNIX time in seconds
std::string current_unix_time() {
    time_t now = time(nullptr);
    return std::to_string(now);
}

void handle_interrupt(int) {
    running = false;
    std::cout << "Heartbeat logger stopped.\n";
    exit(0);
}

int main() {
    signal(SIGINT, handle_interrupt);

    while (running) {
        std::string timestamp = current_unix_time();
        std::ofstream log_file(LOG_FILE_PATH, std::ios::app);
        if (log_file.is_open()) {
            log_file << timestamp << " | OK\n";
            log_file.close();
        } else {
            std::cerr << "Failed to write to log file.\n";
        }

        sleep(1);
    }

    return 0;
}
