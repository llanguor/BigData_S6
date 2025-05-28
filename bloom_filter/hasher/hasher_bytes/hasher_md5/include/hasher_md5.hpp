#pragma once
#include <string>
#include <array>
#include <openssl/md5.h>
#include "hasher.hpp"

template <typename datatype>
class hasher_md5 final:
    public hasher_bytes<datatype>
{
public:

    hasher_md5(): hasher_bytes<datatype>(MD5_DIGEST_LENGTH)
    {
    }

private:

    unsigned char* get_hash_code_from_raw(const void* data, const size_t size) override
    {
        MD5(
            static_cast<const unsigned char*>(data),
            size,
            this->_bufer.get()
        );
        return this->_bufer.get();
    }

};