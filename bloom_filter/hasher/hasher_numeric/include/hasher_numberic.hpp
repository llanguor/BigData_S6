#pragma once
#include "../../include/hasher.hpp"
#include <memory>

template <typename datatype>
class hasher_numeric : public hasher<datatype>
{
public:

    explicit hasher_numeric(const size_t hash_size): hasher<datatype>(hash_size)
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
