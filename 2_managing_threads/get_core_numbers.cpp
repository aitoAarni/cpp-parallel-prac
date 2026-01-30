#include <thread>
#include <iostream>


int main() {
    std::cout << "Hardware cores: " << std::thread::hardware_concurrency() << "\n";
    return 0;
}