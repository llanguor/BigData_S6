#pragma once
#include <list>

#include "collision_strategy.hpp"
#include "../../associative_container/include/key_value_pair.hpp"
#include "hash_provider_numeric.hpp"


template <
    typename tkey,
    typename tvalue>
class collision_chaining_strategy final : public collision_strategy<tkey, tvalue>
{

private:

    std::unique_ptr<std::vector<key_value_pair<tkey,tvalue>>[]> _bits;
    hash_provider_numeric<tkey> * _hash_provider;

public:

    explicit collision_chaining_strategy(
        hash_provider_numeric<tkey> * hash_provider,
        const unsigned long long hash_size):
        collision_strategy<tkey, tvalue>(hash_size * 8),
        _bits(std::make_unique<std::list<key_value_pair<tkey,tvalue>>[]>(collision_strategy<tkey, tvalue>::get_hash_size())),
        _hash_provider(hash_provider)
        {

        }

public:
    void insert(
    tkey const &key,
    tvalue const &value) override
    {

    }

    void insert(
        tkey const &key,
        tvalue &&value) override
    {

    }

    void update(
        tkey const &key,
        tvalue const &value) override
    {

    }

    void update(
        tkey const &key,
        tvalue &&value) override
    {

    }

    tvalue &obtain(
        tkey const &key) override
    {
        return ;
    }

    tvalue dispose(
        tkey const &key) override
    {
        return ;
    }
};
