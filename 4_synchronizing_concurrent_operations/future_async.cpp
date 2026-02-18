#include <iostream>
#include <future>
#include <string>
#include <format>

std::string get_additon_string(const int a, const int b) {
    return std::to_string(a) + "+" + std::to_string(b);
}

int main() {
    // std::launch::async  makes it launch immediately as a thread
    // it also loses refs, since values are copied to the async constructor
    // like with threads
    auto maths = std::async(std::launch::async, get_additon_string, 6, 7);
    std::cout << maths.get() << "\n";
    return 0;
}
