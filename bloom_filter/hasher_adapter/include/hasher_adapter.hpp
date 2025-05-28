#pragma once
#include <array>
#include "hasher.hpp"

template<typename hasher_type>
class hasher_adapter final
{
public:

    enum hasher_types
    {
        md5,
        sha256,
        sha512
    };

private:

    std::unique_ptr<hasher> _hasher;

    hasher_types _type;

public:

    explicit hasher_adapter(
        hasher_types const type):
        _type(type)
    {
        switch (_type)
        {
            case hasher_types::md5:
                _hasher = std::make_unique<md5>();
                break;
            case hasher_types::sha256:
                _hasher = std::make_unique<sha256>();
                break;
            case hasher_types::sha512:
                _hasher = std::make_unique<sha512>();
                break;
        }
    }

public:

    unsigned long long get_hash_code(
        std::string const & str,
        unsigned long long const n)
    {
        /*if (hasher_type::hash_size==0 || hasher_type::hash_size % 2 != 0)
        {
            throw std::invalid_argument("Hash size must be a multiple of 2 and more than zero");
        }*/

        //Error at compile time. 8 - bits in byte

        static_assert(_hasher->get_hash_size()==0 || _hasher->get_hash_size() % 8 != 0, "Hash size must be a multiple of 8 and more than zero");




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

        unsigned long long const mask = (1ULL << n) - 1;
        return static_cast<int>(hash_combined & mask);
    }
};