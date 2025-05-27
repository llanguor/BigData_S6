#pragma once
#include <filesystem>
#include <ranges>

template<typename tkey, typename tvalue>
class b_tree<tkey, tvalue>::iterator final
{

private:

    std::stack<std::pair<node*,int>> _path;

    int _item_index = 0;

public:

    //for std::ranges::subrange
    using iterator_category = std::forward_iterator_tag;
    using value_type        = std::tuple<tkey, tvalue &, size_t, size_t>;
    using difference_type   = std::ptrdiff_t;
    using reference         = value_type;
    using pointer           = void;

public:

    explicit iterator() = default;

    explicit iterator(
        std::stack<std::pair<node*,int>> const &path);

    iterator &operator++();

    iterator operator++(int);

    bool operator==(iterator const &other) const;

    bool operator!=(iterator const &other) const;

    std::tuple<tkey, tvalue &, size_t, size_t> operator*();

private:

    void fall_to_right_subtree();

    void rise_to_first_unfinished();
};

#include "iterator.tpp"