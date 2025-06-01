#pragma once
#include <format>
#include <list>

#include "collision_strategy.hpp"
#include "collision_strategy_concrete.hpp"
#include "../../associative_container/include/key_value_pair.hpp"
#include "hash_provider_numeric.hpp"


template <
    typename tkey,
    typename tvalue>
class collision_chaining_strategy final : public collision_strategy_concrete<tkey, tvalue>
{

private:

    std::unique_ptr<std::vector<key_value_pair<tkey,tvalue>>[]> _bits;
    hash_provider_numeric<tkey> * _hash_provider;
    std::function<bool(tkey const &, tkey const &)> _is_equals;

public:

    explicit collision_chaining_strategy(
        hash_provider_numeric<tkey> & hash_provider,
        std::function<bool(tkey const &, tkey const &)> & is_equals,
        const unsigned long long hash_size):
        collision_strategy_concrete<tkey, tvalue>(hash_size),
        _hash_provider(&hash_provider),
        _is_equals(is_equals)
        {
            _bits.reset(new std::vector<key_value_pair<tkey, tvalue>>[hash_size]());
        }

public:

    tvalue &obtain(
        tkey const &key) override
    {
        auto & pairs = _bits.get()[
            collision_strategy_concrete<tkey, tvalue>::get_index(key, _hash_provider)];

        for (auto it = pairs.begin(); it != pairs.end(); ++it)
        {
            if (_is_equals(it->key, key))
            {
                return it->value;
            }
        }

        throw std::out_of_range(
            std::format("key \"{}\" not found", key));
    }

    tvalue dispose(
        tkey const &key) override
    {

        auto & pairs = _bits.get()[
            collision_strategy_concrete<tkey, tvalue>::get_index(key, _hash_provider)];

        for (auto it = pairs.begin(); it != pairs.end(); ++it)
        {
            if (_is_equals(it->key, key))
            {
                auto value = it->value;
                pairs.erase(it);
                return value;
            }
        }

        throw std::out_of_range(
            std::format("key \"{}\" not found", key));
    }

private:

    void insert(
         tkey const &key,
         tvalue &&value) override
    {
        _bits.get()[
            collision_strategy_concrete<tkey, tvalue>::get_index(key, _hash_provider)]
            .emplace_back(key, std::move(value));
    }
};
