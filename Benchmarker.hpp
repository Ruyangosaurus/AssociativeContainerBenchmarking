#ifndef BENCHMARKER_CLASS_DEFINED
#define BENCHMARKER_CLASS_DEFINED

#include "Measurer.hpp"
#include <fstream>
#include <iostream>
#include <filesystem>
#include "type_prints.hpp"

template<typename T>
T from_size_t(std::size_t);

template<std::constructible_from<std::size_t> T>
T from_size_t(std::size_t i){return T(i);}

template<>
std::string from_size_t<std::string>(std::size_t i){return std::to_string(i);}

template<class key_type, class mapped_type, std::size_t batch_size>
class Benchmarker{
    std::ofstream outfile;
    std::vector <std::pair<key_type, mapped_type>> elements;
    std::random_device::result_type seed;

    template<testable ContainerType>
    void measure_type();
public:
    Benchmarker();
    void operator()();
};

template <class key_type, class mapped_type, std::size_t batch_size>
Benchmarker<key_type, mapped_type, batch_size>::Benchmarker() : seed{std::random_device()()}
{
    std::filesystem::path filename = std::filesystem::current_path() / out_dir / (type_print<key_type>()() + std::string("_") + type_print<mapped_type>()() + std::string("_") + std::to_string(batch_size) + out_format);
    outfile = {filename, std::ios::out | std::ios::trunc};
    for (std::size_t i =0; i < 2*batch_size; ++i){
        elements.emplace_back(from_size_t<key_type>(i), from_size_t<mapped_type>(i));
    }
}

template <class key_type, class mapped_type, std::size_t batch_size>
void Benchmarker<key_type, mapped_type, batch_size>::operator()()
{
    outfile << "container type," << passing_insertions << ',' << failed_insertions << ',' << passing_lookups << ',' << failed_lookups << ',' << passing_searches << ',' << failed_searches << ',' << passing_deletions << ',' << failed_deletions << '\n';
    measure_type<std::map<key_type, mapped_type>>();
    std::cerr << "map done\n";
    measure_type<std::unordered_map<key_type, mapped_type>>();
    std::cerr << "unordered_map done\n";
    measure_type<csaur::BPlusTree<key_type, mapped_type, 256>>();
    std::cerr << "csaur::BPlusTree256 done\n";
    measure_type<csaur::BPlusTree<key_type, mapped_type, 256, csaur::BankAllocator<mapped_type, 146>>>();
    std::cerr << "csaur::BPlusTree256b done\n";
    measure_type<csaur::BPlusTree<key_type, mapped_type, 256, kartoffle::SegmentedFreelistAllocator<mapped_type>>>();
    std::cerr << "csaur::BPlusTree256c done\n";
    measure_type<kartoffle::BPlusTree<key_type, mapped_type, 256, csaur::DefaultAllocator<mapped_type>>>();
    std::cerr << "kartoffle::BPlusTree256 done\n";
    measure_type<kartoffle::BPlusTree<key_type, mapped_type, 256, csaur::BankAllocator<mapped_type, 146>>>();
    std::cerr << "kartoffle::BPlusTree256b done\n";
    measure_type<kartoffle::BPlusTree<key_type, mapped_type, 256>>();
    std::cerr << "kartoffle::BPlusTree256c done\n";
}

template <class key_type, class mapped_type, std::size_t batch_size>
template <testable ContainerType>
inline void Benchmarker<key_type, mapped_type, batch_size>::measure_type()
{
    int status;
    auto measures = Measurer<ContainerType, batch_size>(elements)({seed});
    outfile << type_print<ContainerType>()() << ',' << measures[passing_insertions].count();
    outfile << ',' << measures[failed_insertions].count();
    outfile << ',' << measures[passing_lookups].count();
    outfile << ',' << measures[failed_lookups].count();
    outfile << ',' << measures[passing_searches].count();
    outfile << ',' << measures[failed_searches].count();
    outfile << ',' << measures[passing_deletions].count();
    outfile << ',' << measures[failed_deletions].count();
    outfile << '\n';
}

#endif