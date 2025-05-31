#pragma once


template <typename datatype>
class hash_provider
{

protected:

    size_t const _hash_size;

public:

    explicit hash_provider(
        size_t const hash_size):
        _hash_size(hash_size)
    {
        if (_hash_size==0 || _hash_size % 8 != 0)
        {
            throw std::invalid_argument("Hash size must be a multiple of 2 and more than zero");
        }
    }

public:

    [[nodiscard]] size_t get_hash_size() const
    {
        return _hash_size;
    }

public:

    virtual ~hash_provider() = default;
};