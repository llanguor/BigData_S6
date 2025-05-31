#pragma once
#include "collision_chaining_strategy.hpp"

template <
    typename tkey,
    typename tvalue>
class collision_multihash_strategy final : public collision_strategy<tkey, tvalue>
{
public:



public:

    virtual void insert(
    tkey const &key,
    tvalue const &value)
    {

    }

    virtual void insert(
        tkey const &key,
        tvalue &&value)
    {

    }

    virtual void update(
        tkey const &key,
        tvalue const &value)
    {

    }

    virtual void update(
        tkey const &key,
        tvalue &&value)
    {

    }

    virtual tvalue &obtain(
        tkey const &key)
    {

    }

    virtual tvalue dispose(
        tkey const &key)
    {

    }
};
