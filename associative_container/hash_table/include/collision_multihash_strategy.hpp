#pragma once
#include "collision_strategy.hpp"

template <
    typename tkey,
    typename tvalue>
class collision_multihash_strategy final : public collision_strategy<tkey, tvalue>
{

private:

    class hash_item
    {
    public:

        key_value_pair<tkey, tvalue> key_value;

        std::vector<unsigned long long> hash_codes;

    public:

        explicit hash_item(tkey key, tvalue value):
        key_value(key, std::move(value))
        {

        }
    };

private:

    std::unique_ptr<hash_item<tkey,tvalue>[]> _bits;
    std::vector<hash_provider_numeric<tkey> *> _hash_providers;

public:

    explicit collision_multihash_strategy(
         std::vector<hash_provider_numeric<tkey> *> hash_providers,
        const unsigned long long hash_size):
        collision_strategy<tkey, tvalue>(hash_size * 8),
        _bits(std::make_unique<hash_item<tkey,tvalue>[]>(collision_strategy<tkey, tvalue>::get_hash_size())),
        _hash_providers(hash_providers)
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
