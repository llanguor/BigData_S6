#pragma once
#include <algorithm>

template<
    typename tkey,
    typename tvalue>
struct key_value_pair final
{

public:

    tkey key;
    tvalue value;

public:

    key_value_pair() = default;

    key_value_pair(
        tkey const &key,
        tvalue const &value):
        key(key),
        value(value)
    {

    }

    key_value_pair(
        tkey const &key,
        tvalue &&value):
        key(key),
        value(std::move(value))
    {

    }

};

template<
    typename tkey,
    typename tvalue>
struct key_value_ptr_pair
{

public:

    tkey key;
    tvalue *value_ptr;

public:

    key_value_ptr_pair(
        tkey const &key,
        tvalue *value_ptr):
        key(key),
        value_ptr(value_ptr)
    {

    }

};