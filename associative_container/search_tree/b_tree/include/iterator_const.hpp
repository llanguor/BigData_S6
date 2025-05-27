#pragma once
#include "b_tree.hpp"

template<typename tkey, typename tvalue>
class b_tree<tkey, tvalue>::iterator_const final
{

protected:

    iterator _inner;

public:

    explicit iterator_const(
        std::stack<node *> const &path):
            _inner(path)
    {
    }

    iterator_const &operator++()
    {
        ++_inner;
        return *this;
    }

    iterator_const operator++(
        int)
    {
        auto this_copy = *this;
        ++*this;
        return this_copy;
    }

    bool operator==(
        iterator_const const &other) const
    {
        return _inner == other._inner;
    }

    bool operator!=(
        iterator_const const &other) const
    {
        return !(*this == other);
    }

    std::tuple<tkey, tvalue const &, size_t, size_t> operator*()
    {
        auto [
            key,
            value_ref,
            a,
            b] = *_inner;

        return std::make_tuple(
            key,
            static_cast<tvalue const &>(value_ref),
            a,
            b);
    }
};