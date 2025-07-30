#include <iostream>
#include <chrono>
#include <thread>
#include <gpiod.h>

#define CHIP_NAME "gpiochip4"  // Adjust depending on GPIO mapping
#define LINE_NUM 17            // BCM GPIO17

int main() {
    gpiod_chip* chip = gpiod_chip_info_get_name(CHIP_NAME);
    if (!chip) {
        std::cerr << "Failed to open GPIO chip\n";
        return 1;
    }
    
    gpiod_line* line = gpiod_chip_get_line(chip, LINE_NUM);
    
    
}
