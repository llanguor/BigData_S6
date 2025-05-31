#pragma once

template <
    typename tkey,
    typename tvalue>
class collision_strategy
{
public:

    virtual ~collision_strategy() noexcept = default;

public:

    virtual void insert(
        tkey const &key,
        tvalue const &value) = 0;

    virtual void insert(
        tkey const &key,
        tvalue &&value) = 0;

    virtual void update(
        tkey const &key,
        tvalue const &value) = 0;

    virtual void update(
        tkey const &key,
        tvalue &&value) = 0;

    virtual tvalue &obtain(
        tkey const &key) = 0;

    virtual tvalue dispose(
        tkey const &key) = 0;

};