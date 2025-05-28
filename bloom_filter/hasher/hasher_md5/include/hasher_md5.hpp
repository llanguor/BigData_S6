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

private:

    unsigned char* get_hash_raw(const void* data, size_t size) override
    {
        MD5(
            static_cast<const unsigned char*>(data),
            size,
            this->_bufer.get()
        );
        return this->_bufer.get();
    }

};