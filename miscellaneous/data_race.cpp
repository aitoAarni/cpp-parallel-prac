#include <atomic>
#include <thread>
#include <iostream>
#include <vector>

// if run multiple times, it prints data.a != data.b

struct Data {
    int a;
    int b;
};

std::atomic<int> ready = 0;
std::atomic<bool> stop = false;
long t_count {std::thread::hardware_concurrency() - 1};
Data final_value = {0, 0};


std::ostream& operator<<(std::ostream& os, const Data& data) {
    return os << "Data.a: " << data.a << ", Data.b: " << data.b;
}

template <typename Container, typename Value>
bool number_in_range(const Container& container, const Value value) {
    for (const auto& item : container) {
        if (item == value) {
            return true;
        }
    }
    return false;
}

void thread_func(const int my_num, Data& glob_var, const std::vector<int>& numbers) {
    ready += 1;
    while (ready != t_count) {}
    int a = {my_num};
    int b = {my_num};
    while (a != b && !stop) {
        glob_var.a = my_num;
        glob_var.b = my_num;
        a = glob_var.a;
        b = glob_var.b;
    }
    if (!stop) {
        stop = true;
        final_value.a = a;
        final_value.b = b;
    }
    return;
    
}


int main() {
    std::cout << "threads: " << t_count << "\n";
    Data glob_var {0, 0};
    std::vector<std::jthread> threads;
    std::vector<int> numbers;
    int thread_num {0};
    for (int i {0}; i < t_count; i++) {
        thread_num = i;
        numbers.push_back(thread_num);
        std::cout << "thread_num: " << thread_num << "\n";
        threads.emplace_back(thread_func, thread_num, std::ref(glob_var), std::ref(numbers));
    }
    for (auto& t : threads) {
        t.join();
    }
    std::cout << "final_value: " << final_value << "\n";
    return 0;
}