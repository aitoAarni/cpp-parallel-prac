#include <iostream>
#include <vector>
#include <thread>
#include <future>
#include <string>
#include <chrono>

void set_data_thread(std::promise<std::string>& s) {
    std::string input {};
    std::getline(std::cin, input);
    s.set_value(input);
}

int main() {
    std::promise<std::string> promise_string;
    auto future_string {promise_string.get_future()};

    std::jthread proimse_thread(set_data_thread, std::ref(promise_string));

    while (future_string.wait_for(std::chrono::seconds(0)) != std::future_status::ready) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "waiting for input..." << "\n";
    }

    future_string.wait(); // not necessary since we already confirmed before it is ready
    std::cout << "You wrote: " << future_string.get() << "\n";
    return 0;
}