#pragma once
#include <array>
#include "hasher.hpp"


class hasher_adapter final
{
private:

    std::shared_ptr<hasher> _hasher;

    size_t _hash_size;

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
        unsigned long long hash_combined,
        unsigned long long const n)
    {
        if (n >= 64 || n <=0 )
        {
            throw std::invalid_argument("n must be less than 64 (unsigned long long) and more than 0");
        }

        unsigned long long const mask = (1ULL << n) - 1;
        return hash_combined & mask;
    }


    unsigned long long get_hash_code(
        std::string const & str,
        unsigned long long const n)
    {
        if (n >= 64 || n <=0 )
        {
            throw std::invalid_argument("n must be less than 64 (unsigned long long) and more than 0");
        }

        auto hash =  _hasher->get_hash_code(str);

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

        return get_hash_code(hash_combined, n);
    }
};