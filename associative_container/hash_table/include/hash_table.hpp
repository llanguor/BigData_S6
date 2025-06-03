#pragma once
#include <algorithm>
#include <functional>
#include <memory>
#include <vector>
#include "../../associative_container/include/associative_container.hpp"
#include "collision_chaining_strategy.hpp"
#include "collision_cuckoo_strategy.hpp"
#include "collision_multihash_strategy.hpp"
#include "collision_strategy.hpp"

//TODO: mutex

template <
    typename tkey,
    typename tvalue>
class hash_table final: public associative_container<tkey, tvalue>
{
private:

    std::unique_ptr<collision_strategy<tkey, tvalue>> _collision_strategy;

private:

    explicit hash_table(
        std::unique_ptr<collision_strategy<tkey, tvalue>> && strategy)
    {
        _collision_strategy = std::move(strategy);
    }


public:

    explicit hash_table(
        hash_provider_numeric<tkey> & hash_provider,
        std::function<bool(tkey const &, tkey const &)> is_equals,
        const unsigned long long hash_size)
    {
        _collision_strategy = std::make_unique<collision_chaining_strategy<tkey, tvalue>>(
            hash_provider,
            is_equals,
            hash_size
        );
    }

public:

    static hash_table сreate_with_chaining(
        hash_provider_numeric<tkey> && hash_provider,
        std::function<bool(tkey const &, tkey const &)> is_equals,
        const unsigned long long hash_size)
    {
        return hash_table<tkey, tvalue>(
            std::make_unique<collision_chaining_strategy<tkey, tvalue>>(
            hash_provider,
            is_equals,
            hash_size
        ));
    }

    static hash_table create_with_multihash(
        std::vector<hash_provider_numeric<tkey> *> && hash_providers,
        std::function<bool(tkey const &, tkey const &)> is_equals,
        const unsigned long long hash_size)
    {
        return hash_table<tkey, tvalue>(
            std::make_unique<collision_multihash_strategy<tkey, tvalue>>(
            hash_providers,
            is_equals,
            hash_size
        ));
    }

    static hash_table create_with_cuckoo(
        hash_provider_numeric<tkey> && hash_provider_1,
        hash_provider_numeric<tkey> && hash_provider_2,
        std::function<bool(tkey const &, tkey const &)> is_equals,
        const unsigned long long hash_size)
    {
        return hash_table<tkey, tvalue>(
            std::make_unique<collision_cuckoo_strategy<tkey, tvalue>>(
            hash_provider_1,
            hash_provider_2,
            is_equals,
            hash_size
        ));
    }

public:

    void insert(
        tkey const &key,
        tvalue const &value) override
    {
        _collision_strategy->insert(key, value);
    }

    void insert(
        tkey const &key,
        tvalue &&value) override
    {
        _collision_strategy->insert(key, value);
    }

    tvalue &obtain(
        tkey const &key) override
    {
        return _collision_strategy->obtain(key);
    }

    tvalue dispose(
        tkey const &key) override
    {
        return _collision_strategy->dispose(key);
    }

private:

    void update(
        tkey const &key,
        tvalue const &value) override
    {
        throw std::logic_error("Update is not supported");
    }

    void update(
        tkey const &key,
        tvalue &&value) override
    {
        throw std::logic_error("Update is not supported");
    }
};
