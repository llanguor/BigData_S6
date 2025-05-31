#pragma once
#include <algorithm>
#include <functional>
#include <memory>
#include <vector>
#include "associative_container.hpp"
#include "collision_strategy.hpp"
#include "collision_chaining_strategy.hpp"
#include "collision_multihash_strategy.hpp"

template <
    typename tkey,
    typename tvalue>
class hash_table final: public associative_container<tkey, tvalue>
{
private:

    std::unique_ptr<collision_strategy<tkey, tvalue>> _collision_strategy;

private:

    explicit hash_table(
        std::unique_ptr<collision_strategy<tkey, tvalue>> & collision_strategy):
        _collision_strategy(std::move(collision_strategy))
    {
    }

public:

    static hash_table сreate_with_chaining()
    {
        return hash_table(std::make_unique<collision_chaining_strategy<tkey, tvalue>>());
    }

    static hash_table create_with_multihash(std::vector<std::function<size_t(const tkey&)>> hashes)
    {
        return hash_table(std::make_unique<collision_multihash_strategy<tkey, tvalue>>(std::move(hashes)));
    }

    static hash_table create_with_cuckoo(size_t num_hashes, size_t max_displacements)
    {
        throw std::runtime_error("Not implemented");
    }

public:

    virtual void insert(
        tkey const &key,
        tvalue const &value) override
    {
        _collision_strategy->insert(key, value);
    }

    virtual void insert(
        tkey const &key,
        tvalue &&value) override
    {
        _collision_strategy->insert(key, value);
    }

    virtual void update(
        tkey const &key,
        tvalue const &value) override
    {
        _collision_strategy->update(key, value);
    }

    virtual void update(
        tkey const &key,
        tvalue &&value) override
    {
        _collision_strategy->update(key, value);
    }

    virtual tvalue &obtain(
        tkey const &key) override
    {
        return _collision_strategy->obtain(key);
    }

    virtual tvalue dispose(
        tkey const &key) override
    {
        return _collision_strategy->dispose(key);
    }
};
