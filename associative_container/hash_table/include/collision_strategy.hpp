#pragma once

template <
    typename tkey,
    typename tvalue>
class collision_strategy
{
private:

    unsigned long long _size_in_bits;

public:

    [[nodiscard]] unsigned long long get_hash_size() const
    {
        return _size_in_bits;
    }

protected:

    void set_hash_size(const unsigned long long new_size)
    {
        _size_in_bits = new_size;
    }

public:

    explicit collision_strategy(
        unsigned long long const size_in_bits):
    _size_in_bits(size_in_bits)
    {
    }

public:

    virtual ~collision_strategy() noexcept = default;

public:

    virtual void insert(
        tkey const &key,
        tvalue const &value) = 0;

    virtual void insert(
        tkey const &key,
        tvalue &&value) = 0;

    virtual tvalue &obtain(
        tkey const &key) = 0;

    virtual tvalue dispose(
        tkey const &key) = 0;

};