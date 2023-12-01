#include <iostream>
#include <string>
#include "BigNumber.h"

using namespace std::string_literals;

int main() {
    BigNumber a("12345678"s), b("3");
    std::cout << (a * b) << std::endl;
}