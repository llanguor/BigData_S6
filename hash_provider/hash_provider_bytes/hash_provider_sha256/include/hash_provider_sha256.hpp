#pragma once
#include <string>
#include <array>
#include <openssl/sha.h>
#include "hash_provider.hpp"

template <typename datatype>
class hash_provider_sha256 final:
    public hash_provider_bytes<datatype>
{
public:

    hash_provider_sha256(): hash_provider_bytes<datatype>(SHA256_DIGEST_LENGTH)
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