#pragma once
#include <string>
#include <array>
#include <openssl/sha.h>
#include "hasher.hpp"


class hasher_sha512:
    public hasher<std::string, SHA512_DIGEST_LENGTH>
{
    std::array<unsigned char, SHA512_DIGEST_LENGTH> get_hash_code(std::string const &input) override
    {
        std::array<unsigned char, SHA512_DIGEST_LENGTH> hash{};

        SHA512(reinterpret_cast<const unsigned char*>(
            input.data()),
            input.size(),
            hash.data());

        return hash;
    }
};