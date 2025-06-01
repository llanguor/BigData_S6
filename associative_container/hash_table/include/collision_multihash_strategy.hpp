#pragma once
#include <coroutine>
#include <optional>

#include "collision_strategy.hpp"

template <
    typename tkey,
    typename tvalue>
class collision_multihash_strategy final : public collision_strategy_concrete<tkey, tvalue>
{

private:

    class hash_item
    {
    public:

        key_value_pair<tkey, tvalue> key_value;

        std::vector<unsigned long long> hash_codes;

    public:

        explicit hash_item(
            tkey key,
            tvalue value,
            std::vector<unsigned long long> && hash_codes):
            key_value(key, std::move(value)),
            hash_codes(hash_codes)
        {

        }
    };

private:

    std::unique_ptr<std::optional<hash_item>[]> _bits;
    std::vector<hash_provider_numeric<tkey> *> _hash_providers;
    std::function<bool(tkey const &, tkey const &)> _is_equals;

public:

    explicit collision_multihash_strategy(
         std::vector<hash_provider_numeric<tkey> *> & hash_providers,
         std::function<bool(tkey const &, tkey const &)> & is_equals,
        const unsigned long long hash_size):
        collision_strategy_concrete<tkey, tvalue>(hash_size),
        _is_equals(is_equals),
        _bits(std::make_unique<std::optional<hash_item>[]>(hash_size)),
        _hash_providers(hash_providers)
    {

    }

public:

    tvalue &obtain(
        tkey const &key) override
    {
        throw std::logic_error("Not implemented");
    }

    tvalue dispose(
        tkey const &key) override
    {
        throw std::logic_error("Not implemented");
    }

private:

    void insert(
         tkey const &key,
         tvalue &&value) override
    {

        std::vector<unsigned long long> hash_codes;

        for (auto & hash_provider : _hash_providers)
        {
            auto hash_code = hash_provider->get_hash_code(key);
            auto index = collision_strategy_concrete<tkey, tvalue>::get_remainder(hash_code);
            hash_codes.push_back(std::move(hash_code));

            if (_bits[index] == std::nullopt)
            {
                _bits[index] = hash_item(key, value);
                return;
            }
        }

        //reorder
        throw std::logic_error("Not implemented");

    }
};
