#ifndef TYPE_PRINT_HEADER_DEFINED
#define TYPE_PRINT_HEADER_DEFINED

#include <string>
#include <string_view>
#include <typeinfo>
#include <map>
#include <unordered_map>

template<typename T>
struct type_print{std::string operator()();};

template<>
struct type_print<char32_t>{std::string operator()(){
    return "uint";
}};
template<>
struct type_print<size_t>{std::string operator()(){
    return "ulonglong";
}};
template<>
struct type_print<double>{std::string operator()(){
    return "double";
}};
template<>
struct type_print<bool>{std::string operator()(){
    return "bool";
}};
template<>
struct type_print<std::string>{std::string operator()(){
    return "string";
}};
template <class key_type, class mapped_type>
struct type_print<std::map<key_type, mapped_type>>{std::string operator()(){
    return std::string("std::map<") + type_print<key_type>()() + '|' + type_print<mapped_type>()() + '>';
}};
template <class key_type, class mapped_type>
struct type_print<std::unordered_map<key_type, mapped_type>>{std::string operator()(){
    return std::string("std::unordered_map<") + type_print<key_type>()() + '|' + type_print<mapped_type>()() + '>';
}};

#endif