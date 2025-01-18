#ifndef DEFINITIONS_CONCEPT_DEFINED
#define DEFINITIONS_CONCEPT_DEFINED
#include <concepts>

template<class Container>
concept insertible = requires(Container c, typename Container::key_type&& k, typename Container::mapped_type&& m){
    {c.emplace(std::move(k),std::move(m))};
};

template<class Container>
concept searchable = requires (Container c, typename Container::key_type k){
    {c.at(k)};
};

template<class Container>
concept containable = requires (Container c, typename Container::key_type k){
    {c.contains(k)} -> std::convertible_to<bool>;
};

template<class Container>
concept eraseable = requires (Container c, typename Container::key_type k){
    {c.erase(k)} -> std::convertible_to<bool>;
};

template<class Container>
concept testable = insertible<Container> && searchable<Container> && containable<Container> && eraseable<Container>;

constexpr char const * out_format = ".csv";
constexpr char const * out_dir = "benchmarks";

constexpr char const * passing_insertions = "passing insertions";
constexpr char const * failed_insertions = "failed insertions";
constexpr char const * passing_lookups = "passing lookups";
constexpr char const * failed_lookups = "failed lookups";
constexpr char const * passing_searches = "passing searches";
constexpr char const * failed_searches = "failed searches";
constexpr char const * failed_deletions = "failed deletions";
constexpr char const * passing_deletions = "passing deletions";

constexpr int num_epochs = 5;

#endif