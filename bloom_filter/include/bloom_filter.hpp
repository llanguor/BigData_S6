#pragma once
#include <algorithm>
#include <functional>
#include <iostream>
#include <memory>
#include <stack>
#include <string>
#include <vector>

#include "hash_provider.hpp"
#include "hash_provider_numeric.hpp"


template<typename datatype>
class bloom_filter final
{

private:

    unsigned long long _size_in_bits;
    std::unique_ptr<bool[]> _bits;
    std::vector<hash_provider_numeric<datatype> *> _hashes;

public:

    explicit bloom_filter(
        std::vector<hash_provider_numeric<datatype> *> & hashes,
        const unsigned long long & size):
        _size_in_bits(size * 8),
        _bits(std::make_unique<bool[]>(_size_in_bits)),
        _hashes(std::move(hashes))
        {
        }

public:

    void append(datatype const & value)
    {
        auto result = get_remainder(
            _hashes.at(0)->get_hash_code(value));

        _bits[result] = true;

        for (int i = 1; i < _hashes.size(); ++i)
        {
            result = get_remainder(
                _hashes.at(i)->get_hash_code(result));

            _bits[result] = true;
        }
    }

    bool find(datatype const & value)
    {
        auto result = get_remainder(
            _hashes.at(0)->get_hash_code(value));

        for (int i = 1; i < _hashes.size(); ++i)
        {
            result = get_remainder(
                _hashes.at(i)->get_hash_code(result));

            if (!_bits[result])
            {
                return false;
            }
        }

        return true;
    }

private:

    [[nodiscard]] unsigned long long get_remainder(
     unsigned long long hash_combined) const
    {
        unsigned long long const shift = std::countr_zero(_size_in_bits); //count zero on right side == log2n
        unsigned long long const mask = (1ULL << shift) - 1;
        return hash_combined & mask;
    }

};
