#pragma once
#include <format>
#include <functional>
#include <list>
#include <vector>

#include "collision_strategy.hpp"
#include "collision_strategy_concrete.hpp"
#include "../../associative_container/include/key_value_pair.hpp"
#include "hash_provider_numeric.hpp"
#include <cmath>

template <
    typename tkey,
    typename tvalue>
class collision_cuckoo_strategy final : public collision_strategy_concrete<tkey, tvalue>
{

private:

    class hash_item : public key_value_pair<tkey, tvalue>
    {
    public:

        unsigned long long hash_code_1 = -1;
        unsigned long long hash_code_2 = -1;

    public:

        explicit hash_item(
            tkey key,
            tvalue value,
            unsigned long long hash_code_1,
            unsigned long long hash_code_2):
            hash_code_1(hash_code_1),
            hash_code_2(hash_code_2),
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
    unsigned int max_enclosure = 128;
    std::unique_ptr<std::optional<hash_item>[]> _bits;
    hash_provider_numeric<tkey> * _hash_provider_1;
    hash_provider_numeric<tkey> * _hash_provider_2;
    std::function<bool(tkey const &, tkey const &)> _is_equals;

public:

    explicit collision_cuckoo_strategy(
         hash_provider_numeric<tkey> & hash_provider_1,
         hash_provider_numeric<tkey> & hash_provider_2,
         std::function<bool(tkey const &, tkey const &)> & is_equals,
        const unsigned long long hash_size):
        collision_strategy_concrete<tkey, tvalue>(nearest_power_up(hash_size, _prime)),
        _bits(std::make_unique<std::optional<hash_item>[]>(collision_strategy<tkey, tvalue>::get_hash_size())),
        _is_equals(is_equals),
        _hash_provider_1(&hash_provider_1),
        _hash_provider_2(&hash_provider_2)
    {
    }

public:

    tvalue &obtain(
        tkey const &key) override
    {
        auto index = collision_strategy_concrete<tkey, tvalue>::get_index(key, _hash_provider_1);
        if (_bits[index] != std::nullopt &&
            _is_equals(_bits[index].value().key, key))
        {
            return _bits[index]->value;
        }

        index = collision_strategy_concrete<tkey, tvalue>::get_index(key, _hash_provider_2);
        if (_bits[index] != std::nullopt &&
            _is_equals(_bits[index].value().key, key))
        {
            return _bits[index]->value;
        }

        throw std::out_of_range(
           std::format("key \"{}\" not found", key));
    }

    tvalue dispose(
        tkey const &key) override
    {
        auto index = collision_strategy_concrete<tkey, tvalue>::get_index(key, _hash_provider_1);
        if (_bits[index] != std::nullopt &&
            _is_equals(_bits[index].value().key, key))
        {
            auto value = std::move(_bits[index]->value);
            _bits[index] = std::nullopt;
            return value;
        }

        index = collision_strategy_concrete<tkey, tvalue>::get_index(key, _hash_provider_2);
        if (_bits[index] != std::nullopt &&
            _is_equals(_bits[index].value().key, key))
        {
            auto value = std::move(_bits[index]->value);
            _bits[index] = std::nullopt;
            return value;
        }

        throw std::out_of_range(
           std::format("key \"{}\" not found", key));
    }

private:

    void insert(
         tkey const &key,
         tvalue &&value) override
    {
        auto result = insert_item(key, std::move(value));
        if (result != std::nullopt)
        {
            hashes_reorder();
            insert(result->key, std::move(result->value));
        }
    }

private:

    std::optional<hash_item> insert_item(
         tkey const &key,
         tvalue &&value)
    {
        auto hash_1 = _hash_provider_1->get_hash_code(key);
        auto hash_2 = _hash_provider_2->get_hash_code(key);
        hash_item item(std::move(key), std::move(value), hash_1, hash_2);

        auto index_1 = collision_strategy_concrete<tkey, tvalue>::get_remainder(hash_1);
        unsigned long long start_index = index_1;
        if (_bits[index_1] == std::nullopt ||
            _is_equals(_bits[index_1].value().key, key))
        {
            _bits[index_1] = std::move(item);
            return std::nullopt;
        }

        auto index_2 = collision_strategy_concrete<tkey, tvalue>::get_remainder(hash_1);
        if (_bits[index_2] == std::nullopt ||
            _is_equals(_bits[index_2].value().key, key))
        {
            _bits[index_2] = std::move(item);
            return std::nullopt;
        }

        for (int i=0; i<max_enclosure; ++i)
        {
            std::swap(item, _bits[index_1].value());
            index_2 =  collision_strategy_concrete<tkey, tvalue>::get_remainder(item.hash_code_2);

            if (_bits[index_2] == std::nullopt)
            {
                _bits[index_2] = std::move(item);
                return std::nullopt;
            }

            index_1 = index_2;
        }

        return item;
    }

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

            //can't do std::move because if an error occurs the original
            //image of 'old_bits' will be changed. We can't restore it
            //if 'else' block (repeated hash_reorder)
            auto insert_result = insert_item(old_bit->key,
                tvalue(old_bit->value));

            if (insert_result != std::nullopt)
            {

                _bits = std::move(old_bits);
                increase_degree();
                hashes_reorder();
            }
        }
    }

};