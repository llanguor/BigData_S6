#pragma once
#include <string>
#include <array>
#include <stdexcept>
#include <openssl/md5.h>

template<typename hasher_type>
class hasher_adapter final
{
    int get_hash_code(
        const std::array<
            unsigned char,
            hasher_type::hash_size>& hash,
        int const n)
    {
        /*if (hasher_type::hash_size==0 || hasher_type::hash_size % 2 != 0)
        {
            throw std::invalid_argument("Hash size must be a multiple of 2 and more than zero");
        }*/

        //Error at compile time. 8 - bits in byte
        static_assert(hasher_type::hash_size==0 || hasher_type::hash_size % 8 != 0, "Hash size must be a multiple of 8 and more than zero");

        uint64_t hash_combined = 0;

        for (int i = 0; i < hasher_type::hash_size; i+=8)
        {
            uint64_t hash_8_bits = 0;
            for (int j = i; j < i + 8; ++j)
            {
                hash_8_bits = (hash_8_bits << 8) | hash[j];
            }
            hash_combined ^= hash_8_bits;
        }

        uint64_t const mask = (1ULL << n) - 1;
        return static_cast<int>(hash_combined & mask);
    }
};