#include <iostream>
#include "Benchmarker.hpp"

int main(){
    auto start = std::chrono::steady_clock::now();    
    Benchmarker<std::string, char32_t, 0x0020000>()();
    Benchmarker<char32_t, char32_t, 0x0020000>()();
    Benchmarker<double, double, 0x0020000>()();
    Benchmarker<size_t, bool, 0x0020000>()();
    auto end = std::chrono::steady_clock::now();
    std::cout << "\ntotal runtime: " << std::chrono::hh_mm_ss(end-start) << '\n';
    return 0;
}