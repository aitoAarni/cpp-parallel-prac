#include <iostream>
#include <thread>

void print_hello(const char* name) {
    if (name) {
        std::cout << "Hello Mr." << name << " have a pleasant day\n";
    }
}

int main() {
    std::thread thread1 {print_hello, "bro man"};
    thread1.join();
    return 0;
}