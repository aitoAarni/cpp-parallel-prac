#include <iostream>
#include <thread>
#include <chrono>

void f() {
    std::cout << "thread id: " << std::this_thread::get_id() << "\n";
}
void g() {
    int i = 0;
}

int main() {
    std::thread t1 {f};
    std::thread t2 {g};
    t1.detach();
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "main thread: " << std::this_thread::get_id() << "\n";
    std::cout << "t2 id: " << t2.get_id() << "\n";
    t2.join();

    std::cout << "t2 id after no associated running thread: " << t2.get_id() << "\n";
    return 0;
}