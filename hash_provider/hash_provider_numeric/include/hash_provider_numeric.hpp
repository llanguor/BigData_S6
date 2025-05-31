#pragma once
#include "hash_provider.hpp"
#include <memory>

template <typename datatype>
class hash_provider_numeric : public hash_provider<datatype>
{
public:

    explicit hash_provider_numeric(const size_t hash_size): hash_provider<datatype>(hash_size)
    {
    }

public:

    virtual unsigned long long get_hash_code_from_raw(void const * data, size_t size) = 0;

    virtual unsigned long long get_hash_code(datatype const & input)
    {
        return get_hash_code_from_raw(input.data(), input.size());
    }

    virtual unsigned long long get_hash_code(unsigned long long const & input)
    {
        return get_hash_code_from_raw(&input, sizeof(input));
    }
};
