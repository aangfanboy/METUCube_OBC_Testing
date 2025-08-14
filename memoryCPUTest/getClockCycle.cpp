#include <iostream>
#include <chrono>

int main() {
    auto now = std::chrono::high_resolution_clock::now();
    auto ns  = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count();

    std::cout << "Time since epoch: " << ns << " ns\n";
    return 0;
}
