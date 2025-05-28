#pragma once
#include <array>
#include "hasher.hpp"
#include "../../hasher/hasher_bytes/include/hasher_bytes.hpp"
#include "../../hasher/hasher_numeric/include/hasher_numberic.hpp"

template <typename datatype>
class hasher_bytes_adapter final: public hasher_numeric<datatype>
{
private:

    std::shared_ptr<hasher_bytes<datatype>> _hasher;

public:

    explicit hasher_bytes_adapter(
        const std::shared_ptr<hasher_bytes<datatype>> &hasher):
         hasher_numeric<datatype>(hasher->get_hash_size()),
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
            unsigned long long hash_8_bits = 0;
            for (int j = i; j < i + 8; ++j)
            {
                hash_8_bits = (hash_8_bits << 8) | hash[j];
            }
            hash_combined ^= hash_8_bits;
        }

        return hash_combined;
    }
};
