#include <iostream>
#include <thread>
#include <string>
#include <format>
#include <functional>

using std::string;

class Person {
    private:
        string name;
        int age;
    public:
        Person(string nm, int ag) : name(nm), age(ag) {}

        void greet(string msg) {
            std::cout << std::format("{} greets you with a msg: {}\n", name, msg);
        }
};

void print_ref(const string& str) {
    std::cout << str << "\n";
}

void print_pointer(const string* str) {
    std::cout << *str << "\n";
}

int main() {
    auto p = Person("Lil bro", 15);
    auto p2 = Person("Hampertti", 13);
    std::jthread t1(&Person::greet, &p, "yaa just chilling maaan");
    std::jthread t2(&Person::greet, p2, "No worries man bro");
    string s1 = "A normal reference's data struct will be copied when passed to a thread";
    string s2  {"A pointer's underlying data structure will not be copied"};
    std::jthread t3(print_ref, std::ref(s1));
    std::jthread t4(print_ref, s1); // here the s1 is copied, and not passed by ref
    std::jthread t5(print_pointer, &s2);
    return 0;
}
