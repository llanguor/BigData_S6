#pragma once
#include <list>

#include "collision_strategy.hpp"
#include "../../associative_container/include/key_value_pair.hpp"
#include "hash_provider_numeric.hpp"


template <
    typename tkey,
    typename tvalue>
class collision_strategy_concrete :
    public collision_strategy<tkey, tvalue>
{

public:

    explicit collision_strategy_concrete(
        unsigned long long const size_in_bits):
    collision_strategy<tkey,tvalue>(size_in_bits)
    {
    }

public:

    void insert(
       tkey const &key,
       tvalue const &value) override
    {
        insert(key, tvalue(value));
    }

    void insert(
        tkey const &key,
        tvalue &&value) override = 0;

protected:

    [[nodiscard]] unsigned long long get_remainder(
     unsigned long long hash) const
    {
        /*
        unsigned long long const shift = std::countr_zero(
            collision_strategy<tkey, tvalue>::get_hash_size());
        unsigned long long const mask = (1ULL << shift) - 1;
        return hash & mask;
        */

        return hash % collision_strategy<tkey, tvalue>::get_hash_size(); //because the number does not have to be a power of two
    }

    virtual unsigned long long
        get_index(tkey const &key, hash_provider_numeric<tkey> * hash_provider)
    {
        return collision_strategy_concrete<tkey, tvalue>::get_remainder(
               hash_provider->get_hash_code(key)
               );
    }
};
