#include <iostream>
#include <functional>
using namespace std;

template <size_t N>
using Chars = char[N];

using CharPtr = char*;
using CCharPtr = const char*;

template <size_t N>
void Print(const Chars<N>&) {
    std::cout << "Call Print[N]" << std::endl;
}

void Print(CCharPtr&) {
    std::cout << "Call Print const char*" << std::endl;
}

void Print(CharPtr&) {
    std::cout << "Call Print char*" << std::endl;
}

int i = 0;

int main(int argc, char** argv) {
    char* str = new char[10]{0};
    str[0] = 'h';
    str[1] = 'l';
    str[2] = 'l';
    str[3] = 'o';
    const char* c1 = "hello world";
    const char c2[] = "hello world2";

    std::cout << "Print const char*" << std::endl;
    Print(c1);

    std::cout << "Print const char[]" << std::endl;
    Print(c2);

    std::cout << "Print char*" << std::endl;
    Print(str);

    std::cout << "Print constant string" << std::endl;
    Print("hello world, too");

    delete[] str;
    return 0;
}
