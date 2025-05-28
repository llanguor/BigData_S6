#pragma once
#include <string>
#include <array>
#include <openssl/sha.h>
#include "hasher.hpp"


class hasher_sha256 final:
    public hasher
{
public:

    hasher_sha256(): hasher(SHA256_DIGEST_LENGTH)
    {
    }

public:

    unsigned char * get_hash_code(std::string const &input) override
    {
        SHA256(reinterpret_cast<const unsigned char*>(
            input.data()),
            input.size(),
            _bufer.get());

        return _bufer.get();
    }

    unsigned char * get_hash_code(unsigned long long const &input) override
    {
        SHA256(reinterpret_cast<const unsigned char*>(&input),
           sizeof(unsigned long long),
           _bufer.get());

        return _bufer.get();
    }
};