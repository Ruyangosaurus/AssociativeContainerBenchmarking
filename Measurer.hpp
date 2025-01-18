#ifndef MEASURER_CLASS_DEFINED
#define MEASURER_CLASS_DEFINED
#include "definitions.hpp"
#include <fstream>
#include <chrono>
#include <map>
#include <vector>
#include <random>
#include <algorithm>

template<testable ContainerType, std::size_t batch_size>
class Measurer{
    std::vector <std::pair<typename ContainerType::key_type, typename ContainerType::mapped_type>> elements; 

    std::pair<std::chrono::nanoseconds, std::chrono::nanoseconds> measure_insertions();
    std::pair<std::chrono::nanoseconds, std::chrono::nanoseconds> measure_searches();
    std::pair<std::chrono::nanoseconds, std::chrono::nanoseconds> measure_lookups();
    std::pair<std::chrono::nanoseconds, std::chrono::nanoseconds> measure_deletions();
public:
    Measurer(const std::vector <std::pair<typename ContainerType::key_type, typename ContainerType::mapped_type>>& elements);
    std::map<std::string, std::chrono::nanoseconds> operator()(std::seed_seq seed);
};

template <testable ContainerType, std::size_t batch_size>
std::pair<std::chrono::nanoseconds, std::chrono::nanoseconds> Measurer<ContainerType, batch_size>::measure_insertions() 
{
    ContainerType dict;
    auto start = std::chrono::steady_clock::now();
    for (int i = 0; i < batch_size; ++i){
        dict.emplace(typename ContainerType::key_type(elements[i].first), typename ContainerType::mapped_type(elements[i].second));
    }
    auto end = std::chrono::steady_clock::now();
    std::chrono::nanoseconds passing = (std::chrono::duration_cast<std::chrono::nanoseconds>(end - start)) / batch_size;
    start = std::chrono::steady_clock::now();
    for (int i = 0; i < batch_size; ++i){
        dict.emplace(typename ContainerType::key_type(elements[i].first), typename ContainerType::mapped_type(elements[i].second));
    }
    end = std::chrono::steady_clock::now();
    std::chrono::nanoseconds failing = (std::chrono::duration_cast<std::chrono::nanoseconds>(end - start)) / batch_size;
    return {passing,failing};
}

template <testable ContainerType, std::size_t batch_size>
std::pair<std::chrono::nanoseconds, std::chrono::nanoseconds> Measurer<ContainerType, batch_size>::measure_lookups() 
{
    ContainerType dict;
    for (int i = 0; i < batch_size; ++i){
        dict.emplace(typename ContainerType::key_type(elements[i].first), typename ContainerType::mapped_type(elements[i].second));
    }
    auto start = std::chrono::steady_clock::now();
    for (int i = 0; i < batch_size; ++i){
        dict.contains(elements[i].first);
    }
    auto end = std::chrono::steady_clock::now();
    std::chrono::nanoseconds passing = (std::chrono::duration_cast<std::chrono::nanoseconds>(end - start)) / batch_size;
    start = std::chrono::steady_clock::now();
    for (int i = batch_size; i < 2*batch_size; ++i){
        dict.contains(elements[i].first);
    }
    end = std::chrono::steady_clock::now();
    std::chrono::nanoseconds failing = (std::chrono::duration_cast<std::chrono::nanoseconds>(end - start)) / batch_size;
    return {passing,failing};
}

template <testable ContainerType, std::size_t batch_size>
std::pair<std::chrono::nanoseconds, std::chrono::nanoseconds> Measurer<ContainerType, batch_size>::measure_searches() 
{
    ContainerType dict;
    for (int i = 0; i < batch_size; ++i){
        dict.emplace(typename ContainerType::key_type(elements[i].first), typename ContainerType::mapped_type(elements[i].second));
    }
    auto start = std::chrono::steady_clock::now();
    for (int i = 0; i < batch_size; ++i){
        dict.at(elements[i].first);
    }
    auto end = std::chrono::steady_clock::now();
    std::chrono::nanoseconds passing = (std::chrono::duration_cast<std::chrono::nanoseconds>(end - start)) / batch_size;
    start = std::chrono::steady_clock::now();
    for (int i = batch_size; i < 2*batch_size; ++i){
        try{
            dict.at(elements[i].first);
        }
        catch(std::exception& e){
            ;
        }
    }
    end = std::chrono::steady_clock::now();
    std::chrono::nanoseconds failing = (std::chrono::duration_cast<std::chrono::nanoseconds>(end - start)) / batch_size;
    return {passing,failing};
}

template <testable ContainerType, std::size_t batch_size>
std::pair<std::chrono::nanoseconds, std::chrono::nanoseconds> Measurer<ContainerType, batch_size>::measure_deletions() 
{
    ContainerType dict;
    for (int i = 0; i < batch_size; ++i){
        dict.emplace(typename ContainerType::key_type(elements[i].first), typename ContainerType::mapped_type(elements[i].second));
    }
    auto start = std::chrono::steady_clock::now();
    for (int i = batch_size; i < 2*batch_size; ++i){
        dict.erase(elements[i].first);
    }
    auto end = std::chrono::steady_clock::now();
    std::chrono::nanoseconds failing = (std::chrono::duration_cast<std::chrono::nanoseconds>(end - start)) / batch_size;
    start = std::chrono::steady_clock::now();
    for (int i = 0; i < batch_size; ++i){
        dict.erase(elements[i].first);
    }
    end = std::chrono::steady_clock::now();
    std::chrono::nanoseconds passing = (std::chrono::duration_cast<std::chrono::nanoseconds>(end - start)) / batch_size;
    return {passing,failing};
}

template <testable ContainerType, std::size_t batch_size>
Measurer<ContainerType, batch_size>::Measurer(const std::vector <std::pair<typename ContainerType::key_type, typename ContainerType::mapped_type>> &elements) : 
    elements(elements)
{
}

template <testable ContainerType, std::size_t batch_size>
std::map<std::string, std::chrono::nanoseconds> Measurer<ContainerType, batch_size>::operator()(std::seed_seq seed)
{
    std::map<std::string, std::chrono::nanoseconds> out;
    for (int i = 0; i < num_epochs; ++i){
        std::shuffle(elements.begin(), elements.end(), std::default_random_engine (seed));
        auto measures = measure_insertions();
        out[passing_insertions] += measures.first;
        out[failed_insertions]  += measures.second;
        measures = measure_lookups();
        out[passing_lookups]    += measures.first;
        out[failed_lookups]     += measures.second;
        measures = measure_searches();
        out[passing_searches]    += measures.first;
        out[failed_searches]     += measures.second;
        measures = measure_deletions();
        out[failed_deletions]   += measures.second;
        out[passing_deletions]  += measures.first;
    }
    for (auto i : out){
        i.second /= num_epochs;
    }
    return out;
}

#endif