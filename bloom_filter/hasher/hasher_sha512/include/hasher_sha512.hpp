#pragma once
#include <string>
#include <array>
#include <openssl/sha.h>
#include "hasher.hpp"

class hasher_sha512 final:
    public hasher
{
public:

    hasher_sha512(): hasher(SHA512_DIGEST_LENGTH)
    {
    }

private:

    unsigned char* get_hash_raw(const void* data, size_t size) override
    {
        SHA512(
            static_cast<const unsigned char*>(data),
            size,
            this->_bufer.get()
        );
        return this->_bufer.get();
    }

};