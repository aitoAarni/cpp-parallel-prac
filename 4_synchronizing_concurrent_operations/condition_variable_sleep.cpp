#include <iostream>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <vector>


std::vector<int> input_data;
std::mutex mt;
std::condition_variable data_cond;
bool flag {false};

void input_thread() {
    int input_int {0};
    while (true) {
        std::cin >> input_int;
        {
            std::unique_lock lock(mt);
            if (input_int == -1) {
                flag = true;
            } else {
                input_data.push_back(input_int);
            }
            data_cond.notify_one();
        }
    if (flag) return;
    }
}

void output_thread() {
    while (!flag) {
        {
            std::unique_lock lock(mt);
            data_cond.wait(lock, []{return input_data.size() != 0 || flag;}); 
            while (input_data.size() > 0) {
                std::cout << "you input " << input_data.back() << "\n";
                input_data.pop_back();
                }

            }

    } 
}

int main() {
    std::jthread t1{ input_thread };
    std::jthread t2{ output_thread };
    return 0;
}

