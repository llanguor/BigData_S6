#pragma once
#include <string>
#include <array>
#include <openssl/md5.h>
#include "hasher.hpp"

//'constexpr': just 'const', but set at compile time
constexpr size_t HASH_SIZE = 16;

class hasher_md5 :
    public hasher<std::string, HASH_SIZE>
{
    std::array<unsigned char, HASH_SIZE> get_hash_code(std::string const &input) override
    {
        std::array<unsigned char, HASH_SIZE> hash{};

        MD5(reinterpret_cast<const unsigned char*>(
            input.data()),
            input.size(),
            hash.data());

        return hash;
    }
};