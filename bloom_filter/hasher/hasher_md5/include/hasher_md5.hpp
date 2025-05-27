#pragma once
#include <string>
#include <array>
#include <openssl/md5.h>
#include "hasher.hpp"

class hasher_md5 :
    public hasher<std::string, MD5_DIGEST_LENGTH>
{
    std::array<unsigned char, MD5_DIGEST_LENGTH> get_hash_code(std::string const &input) override
    {
        std::array<unsigned char, MD5_DIGEST_LENGTH> hash{};

        MD5(reinterpret_cast<const unsigned char*>(
            input.data()),
            input.size(),
            hash.data());

        return hash;
    }
};