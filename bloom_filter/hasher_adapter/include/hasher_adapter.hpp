#pragma once
#include <array>
#include "hasher.hpp"


class hasher_adapter final
{
private:

    std::shared_ptr<hasher> _hasher;

    size_t _hash_size;

private:

    unsigned long long get_remainder(
     unsigned long long hash_combined,
     unsigned long long const n)
    {
        if (n <= 0 || (n & (n - 1)) != 0) //n<=0 and n=2^x
        {
            throw std::invalid_argument("the number must be of the form 2^n and more than 0");
        }

        unsigned long long const shift = std::countr_zero(n); //count zero on right side == log2n
        unsigned long long const mask = (1ULL << shift) - 1;
        return hash_combined & mask;
    }

    unsigned long long get_hash_combined_to_long(
     unsigned char const * hash,
     unsigned long long const n) const
    {
        unsigned long long hash_combined = 0;

        for (int i = 0; i < _hash_size; i+=8)
        {
            unsigned long long hash_8_bits = 0;
            for (int j = i; j < i + 8; ++j)
            {
                hash_8_bits = (hash_8_bits << 8) | hash[j];
            }
            hash_combined ^= hash_8_bits;
        }

        return hash_combined;
    }


public:

    explicit hasher_adapter(const std::shared_ptr<hasher> &hasher):
        _hasher(hasher),
        _hash_size(_hasher->get_hash_size())
    {
        if (_hash_size==0 || _hash_size % 8 != 0)
        {
            throw std::invalid_argument("Hash size must be a multiple of 2 and more than zero");
        }
    }

public:

    unsigned long long get_hash_code(
        unsigned long long data,
        unsigned long long const n)
    {
        auto hash =  _hasher->get_hash_code(data);
        auto hash_combined = get_hash_combined_to_long(hash, n);
        return get_remainder(hash_combined, n);
    }


    unsigned long long get_hash_code(
        std::string const & data,
        unsigned long long const n)
    {
        auto hash =  _hasher->get_hash_code(data);
        auto hash_combined = get_hash_combined_to_long(hash, n);
        return get_remainder(hash_combined, n);
    }
};