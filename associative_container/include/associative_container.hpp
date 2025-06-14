#pragma once
#include <algorithm>
#include "key_value_pair.hpp"

template<
    typename tkey,
    typename tvalue>
class associative_container
{

public:

    using key_value_pair = key_value_pair<tkey, tvalue>;

    using key_value_ptr_pairs = key_value_ptr_pair<tkey, tvalue>;

public:

    virtual ~associative_container() noexcept = default;

public:

    virtual void insert(
        tkey const &key,
        tvalue const &value) = 0;

    virtual void insert(
        tkey const &key,
        tvalue &&value) = 0;

    virtual void update(
        tkey const &key,
        tvalue const &value) = 0;

    virtual void update(
        tkey const &key,
        tvalue &&value) = 0;

    virtual tvalue &obtain(
        tkey const &key) = 0;

    virtual tvalue dispose(
        tkey const &key) = 0;

};
