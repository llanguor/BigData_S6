#pragma once
#include <algorithm>

template<
    typename tkey,
    typename tvalue>
class associative_container
{

public:

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

public:

    virtual ~associative_container() noexcept = default;

public:

    [[nodiscard]] virtual size_t elements_count() const = 0;

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
