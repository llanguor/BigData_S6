#pragma once
#include <string>
#include <array>
#include <openssl/md5.h>
#include "hasher.hpp"


class hasher_md5 final:
    public hasher
{
public:

    hasher_md5(): hasher(MD5_DIGEST_LENGTH)
    {
    }

public:

    unsigned char * get_hash_code(std::string const &input) override
    {
        MD5(reinterpret_cast<const unsigned char*>(
            input.data()),
            input.size(),
            _bufer.get());

        return _bufer.get();
    }

    unsigned char * get_hash_code(unsigned long long const &input) override
    {
        MD5(reinterpret_cast<const unsigned char*>(&input),
           sizeof(unsigned long long),
           _bufer.get());

        return _bufer.get();
    }
};