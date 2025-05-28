#pragma once
#include <string>
#include <array>
#include <openssl/sha.h>
#include "hasher.hpp"

template <typename datatype>
class hasher_sha256 final:
    public hasher_bytes<datatype>
{
public:

    hasher_sha256(): hasher_bytes<datatype>(SHA256_DIGEST_LENGTH)
    {
    }

private:

    unsigned char* get_hash_code_from_raw(const void* data, const size_t size) override
    {
        SHA256(
            static_cast<const unsigned char*>(data),
            size,
            this->_bufer.get()
        );
        return this->_bufer.get();
    }

};