#include <iostream>
#include <vector>
#include <thread>
#include <future>
#include <string>

void set_data_thread(std::promise<std::string>& s) {
    std::string input {};
    std::getline(std::cin, input);
    s.set_value(input);
}

int main() {
    std::promise<std::string> promise_string;
    auto future_string {promise_string.get_future()};

    std::jthread proimse_thread(set_data_thread, std::ref(promise_string));

    future_string.wait(); // not necessary
    std::cout << "You wrote: " << future_string.get() << "\n";
    return 0;
}