#pragma once
#include <vector>
#include "associative_container.hpp"

template<
    typename tkey,
    typename tvalue>
class search_tree:
    public associative_container<tkey, tvalue>
{

public:

    [[nodiscard]] virtual size_t elements_count() const = 0;

public:

    virtual std::vector<typename associative_container<tkey, tvalue>::key_value_pair> obtain_between(
        tkey const &lower_bound,
        tkey const &upper_bound,
        bool lower_bound_inclusive,
        bool upper_bound_inclusive) = 0;

};