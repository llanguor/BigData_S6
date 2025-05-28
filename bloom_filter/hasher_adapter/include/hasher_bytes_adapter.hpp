#pragma once
#include <array>
#include "hasher.hpp"
#include "hasher_bytes.hpp"
#include "hasher_numberic.hpp"

template <typename datatype>
class hasher_bytes_adapter final: public hasher_numeric<datatype>
{
private:

    std::shared_ptr<hasher_bytes<datatype>> _hasher;

    unsigned long long _remainder_degree;

public:

    explicit hasher_bytes_adapter(
        const std::shared_ptr<hasher_bytes<datatype>> &hasher,
        unsigned long long const & remainder_degree):
         hasher_numeric<datatype>(hasher->get_hash_size()),
         _hasher(hasher),
         _remainder_degree(remainder_degree)
    {
    }

public:

    unsigned long long get_hash_code_from_raw(void const * data, size_t size) override
    {
        auto hash = _hasher->get_hash_code_from_raw(data, size);
        auto hash_combined = get_hash_combined_to_long(hash);
        return get_remainder(hash_combined);
    }


private:

    [[nodiscard]] unsigned long long get_remainder(
     unsigned long long hash_combined) const
    {
        if (_remainder_degree <= 0 ||
            (_remainder_degree & (_remainder_degree - 1)) != 0) //n<=0 and n=2^x
        {
            throw std::invalid_argument("the number must be of the form 2^n and more than 0");
        }

        unsigned long long const shift = std::countr_zero(_remainder_degree); //count zero on right side == log2n
        unsigned long long const mask = (1ULL << shift) - 1;
        return hash_combined & mask;
    }

    unsigned long long get_hash_combined_to_long(
     unsigned char const * hash) const
    {
        unsigned long long hash_combined = 0;

        for (int i = 0; i < _hasher->get_hash_size(); i+=8)
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
};
