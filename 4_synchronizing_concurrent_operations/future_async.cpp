#include <iostream>
#include <future>
#include <string>
#include <format>

std::string get_additon_string(const int a, const int b) {
    return std::to_string(a) + "+" + std::to_string(b);
}

int main() {
    // std::launch::async  makes it launch immediately as a thread
    // std::launch::deferred makes it run when get() or wait() memb. funcs are called
    // it also loses refs (like thread), since values are copied to the async constructor
    // like with threads
    std::future<std::string> maths = std::async(
        std::launch::async, get_additon_string, 6, 7
    );

    std::cout << maths.get() << "\n";
    return 0;
}
