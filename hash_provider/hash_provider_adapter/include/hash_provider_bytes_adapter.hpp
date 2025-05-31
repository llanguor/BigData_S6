#pragma once
#include <array>
#include "hash_provider.hpp"
#include "hash_provider_bytes.hpp"
#include "hash_provider_numeric.hpp"

template <typename datatype>
class hash_provider_bytes_adapter final: public hash_provider_numeric<datatype>
{
private:

    std::shared_ptr<hash_provider_bytes<datatype>> _hasher;

public:

    explicit hash_provider_bytes_adapter(
        const std::shared_ptr<hash_provider_bytes<datatype>> &hasher):
         hash_provider_numeric<datatype>(hasher->get_hash_size()),
         _hasher(hasher)
    {
    }

public:

    unsigned long long get_hash_code_from_raw(void const * data, size_t size) override
    {
        auto hash = _hasher->get_hash_code_from_raw(data, size);

        unsigned long long hash_combined = 0;

        for (int i = 0; i < _hasher->get_hash_size(); i+=8)
        {
            unsigned long long hash_8_bytes = 0;
            for (int j = i; j < i + 8; ++j)
            {
                hash_8_bytes = (hash_8_bytes << 8) | hash[j];
            }
            hash_combined ^= hash_8_bytes;
        }

        return hash_combined;
    }
};
