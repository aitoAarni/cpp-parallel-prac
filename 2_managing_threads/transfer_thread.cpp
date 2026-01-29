#include <iostream>
#include <string_view>
#include <thread>

void print_msg(std::string_view name){
    std::cout << "Hello Mr. " << name << "\n";
}

void handle_thread(std::thread t) {
    t.join();
}

int main() {
    std::thread t1 {print_msg, "Mr sir"};
    auto t2 = std::move(t1);
    t2.join();
    t1 = std::thread(print_msg, "Second msg from Mr sir");
    handle_thread(std::move(t1));
    return 0;
}