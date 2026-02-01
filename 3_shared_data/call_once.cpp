#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <algorithm>

 class DoWork {
    private:
    int thread_number;
    void initialize_print() {
        std::cout << "Yee is da first thread calling this, badoom thread: "
        << thread_number << "\n\n";
    }
    void print() {
        std::cout << "Just thread " << thread_number << " doing work\n";
    }

    public:
    inline static std::once_flag first_call;
    inline static std::mutex mutex;
    DoWork(int t_num): thread_number(t_num) {}
    void operator()(){
        std::call_once(first_call, &DoWork::initialize_print, this);
        std::unique_lock lk {mutex};
        print();
    }
 };

int main() {
    std::vector<std::jthread> threads;
    auto cores = static_cast<int>(std::thread::hardware_concurrency());
    for (int i = 1; i < std::max(2, cores); i++) {
        auto do_work = DoWork(i);
        threads.emplace_back(do_work);
    }
    
    return 0;
}