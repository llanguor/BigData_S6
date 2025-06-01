#pragma once
#include <coroutine>
#include <optional>
#include <cmath>

#include "collision_strategy.hpp"

template <
    typename tkey,
    typename tvalue>
class collision_multihash_strategy final : public collision_strategy_concrete<tkey, tvalue>
{

private:

    class hash_item : public key_value_pair<tkey, tvalue>
    {
    public:

        std::vector<unsigned long long> hash_codes;

    public:

        explicit hash_item(
            tkey key,
            tvalue value,
            std::vector<unsigned long long> && hash_codes):
            hash_codes(hash_codes),
            key_value_pair<tkey, tvalue>(std::move(key), std::move(value))
        {

        }

        explicit hash_item(
            tkey key,
            tvalue value):
            key_value_pair<tkey, tvalue>(std::move(key), std::move(value))
        {

        }
    };

private:

    unsigned static constexpr int _prime = 5;
    std::unique_ptr<std::optional<hash_item>[]> _bits;
    std::vector<hash_provider_numeric<tkey> *> _hash_providers;
    std::function<bool(tkey const &, tkey const &)> _is_equals;

public:

    explicit collision_multihash_strategy(
         std::vector<hash_provider_numeric<tkey> *> & hash_providers,
         std::function<bool(tkey const &, tkey const &)> & is_equals,
        unsigned long long hash_size):
        collision_strategy_concrete<tkey, tvalue>(nearest_power_up(hash_size, _prime)),
        _bits(std::make_unique<std::optional<hash_item>[]>(collision_strategy<tkey, tvalue>::get_hash_size())),
        _hash_providers(hash_providers),
        _is_equals(is_equals)
    {
    }

public:

    tvalue &obtain(
        tkey const &key) override
    {
        for (auto & hash_provider : _hash_providers)
        {
            auto index = collision_strategy_concrete<tkey, tvalue>::get_index(key, hash_provider);
            if (_bits[index] != std::nullopt && _is_equals(_bits[index]->key, key))
            {
                return _bits[index]->value;
            }
        }

        throw std::out_of_range(
            std::format("Key {} not found", key));

    }

    tvalue dispose(
        tkey const &key) override
    {
        for (auto & hash_provider : _hash_providers)
        {
            auto index = collision_strategy_concrete<tkey, tvalue>::get_index(key, hash_provider);
            if (_bits[index] != std::nullopt && _is_equals(_bits[index]->key, key))
            {
                auto value = _bits[index]->value;
                _bits[index] = std::nullopt;
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
        std::vector<unsigned long long> hash_codes;

        for (auto & hash_provider : _hash_providers)
        {
            auto hash_code = hash_provider->get_hash_code(key);
            auto index = collision_strategy_concrete<tkey, tvalue>::get_remainder(hash_code);
            hash_codes.push_back(std::move(hash_code));

            if (_bits[index] == std::nullopt || _is_equals(_bits[index]->key, key))
            {
                _bits[index] = std::move(hash_item(key, value));
                return;
            }
        }

        hashes_reorder();
        insert(key, std::move(value));
    }

private:

    static unsigned long long nearest_power_up(unsigned long long input_size, const unsigned int prime)
    {
        //min degree for prime number
        double power = std::ceil(std::log(input_size) / std::log(prime));
        return static_cast<unsigned long long>(std::pow(prime, power));
    }

    void increase_degree()
    {
        collision_strategy<tkey, tvalue>::set_hash_size(
            collision_strategy<tkey, tvalue>::get_hash_size() * _prime);

        _bits = std::make_unique<std::optional<hash_item>[]>(collision_strategy<tkey, tvalue>::get_hash_size());

    }

    void hashes_reorder()
    {
        auto old_size = collision_strategy<tkey, tvalue>::get_hash_size();
        auto old_bits = std::move(_bits);
        increase_degree();

        for (int i = 0; i < old_size; ++i)
        {
            auto & old_bit = old_bits[i];
            if (old_bit == std::nullopt)
            {
                continue;
            }

            auto old_bit_it = old_bit->hash_codes.begin();
            auto old_bit_it_end = old_bit->hash_codes.end();

            for (int hash_provider_index = 0; i < _hash_providers.size(); ++hash_provider_index)
            {
                unsigned long long hash;
                if (old_bit_it != old_bit_it_end)
                {
                    hash =  *old_bit_it;
                    ++old_bit_it;
                }
                else
                {
                    hash = _hash_providers[i]->get_hash_code(old_bit->key);
                }

                auto new_index = collision_strategy_concrete<tkey, tvalue>::get_remainder(hash);

                if (_bits[new_index]== std::nullopt)
                {
                    _bits[new_index] = std::move(old_bit);
                    break;
                }
                else if (hash_provider_index==_hash_providers.size()-1)
                {
                    _bits = std::move(old_bits);
                    hashes_reorder();
                }
            }
        }
    }
};
