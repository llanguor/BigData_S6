#pragma once
#include <array>

template<
    typename datatype, size_t type_hash_size>
class hasher
{

public:

    using input_type = datatype;
    static constexpr size_t hash_size = type_hash_size;

public:

    //unsigned char for storage bytes
    virtual std::array<unsigned char, type_hash_size> get_hash_code(datatype const & input) = 0;

    virtual ~hasher() = default;

};
