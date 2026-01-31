#include <thread>
#include <mutex>  // scoped_lock
#include <iostream>
#include <functional>  // for std::ref
#include <array>

void f_lock_guard_version(int number, std::mutex& mutex) {
    std::scoped_lock guard{mutex}; // or use std::lock_guard (older)
    std::cout << "In thread: " << std::this_thread::get_id() << "\n"
        << "t num: " << number << "\n";
}

void f_mutex_version(int number, std::mutex& mutex) {
    mutex.lock();
    std::cout << "In thread: " << std::this_thread::get_id() << "\n"
        << "t num: " << number << "\n";
    mutex.unlock();
}

int main() {
    std::mutex print_mutex;
    int thread_num {static_cast<int>(std::thread::hardware_concurrency())};
    std::array<std::thread, 32> threads;

    for (int t = 1; t < thread_num; t++) {
        threads[t] = std::thread(f_lock_guard_version, t, std::ref(print_mutex));
    }

    for (int t = 1; t < thread_num; t++) {
        threads[t].join();
    }

    return 0;
}