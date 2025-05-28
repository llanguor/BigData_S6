#pragma once
#include <array>

class hasher
{
public:

    [[nodiscard]] virtual std::type_info const & get_type_info() const = 0;

    [[nodiscard]] virtual size_t get_hash_size() const = 0;

public:

    virtual ~hasher() = default;

};



template<
    typename datatype, size_t type_hash_size>
class hasher_template : hasher
{

private:

    using _type = datatype;
    const size_t _hash_size = type_hash_size;

public:

    [[nodiscard]] std::type_info const & get_type_info() const override
    {
        return typeid(_type);
    }

    [[nodiscard]] size_t get_hash_size() const override
    {
        return _hash_size;
    }

public:

    //unsigned char for storage bytes
    virtual std::array<unsigned char, type_hash_size> get_hash_code(datatype const & input) = 0;

};
