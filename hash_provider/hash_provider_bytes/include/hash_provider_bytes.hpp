#pragma once
#include "hash_provider.hpp"
#include <memory>

template <typename datatype>
class hash_provider_bytes : public hash_provider<datatype>
{

protected:

    std::shared_ptr<unsigned char[]> _bufer = nullptr;

public:

    explicit hash_provider_bytes(const size_t hash_size): hash_provider<datatype>(hash_size)
    {
        _bufer = std::make_shared<unsigned char[]>(hash_size);
    }

public:

    virtual unsigned char* get_hash_code_from_raw(void const * data, size_t size) = 0;


    virtual unsigned char* get_hash_code(datatype const & input)
    {
        return get_hash_code_from_raw(input.data(), input.size());
    }

    virtual unsigned char* get_hash_code(unsigned long long const & input)
    {
        return get_hash_code_from_raw(&input, sizeof(input));
    }
};
