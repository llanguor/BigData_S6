#pragma once
#include "iterator.hpp"

template<typename tkey, typename tvalue>
b_tree<tkey, tvalue>::iterator::iterator(
     std::stack<std::pair<node*,int>> const &path):
    _path(path)
{
    if (_path.empty())
        return;

    _item_index =
        _path.top().second < 0 ?
        -_path.top().second-1 :
       _path.top().second;
}


template<typename tkey, typename tvalue>
typename b_tree<tkey, tvalue>::iterator & b_tree<tkey, tvalue>::iterator::operator++()
{

    if (_path.empty())
        return *this;

    //on last item
    if (_item_index == _path.top().first->virtual_size-1)
    {
        if (_path.top().first->subtrees[_item_index+1]==nullptr)
        {
            //up to parent-item
            rise_to_first_unfinished();
        }
        else
        {
            //go to last right subtree
            fall_to_right_subtree();
        }

        return *this;
    }

    //after last right subtree returned: up to parent-item
    if (_item_index == _path.top().first->virtual_size)
    {
        rise_to_first_unfinished();
        return *this;
    }

    //on non-last elements
    if (_path.top().first->subtrees[_item_index+1]==nullptr)
    {
        //just go to the right item
        ++_item_index;
        return *this;
    }
    else
    {
        //go to item's right subtree
        fall_to_right_subtree();
        return *this;
    }
}


template<typename tkey, typename tvalue>
typename b_tree<tkey, tvalue>::iterator b_tree<tkey, tvalue>::iterator::operator++(int)
{
    auto this_copy = *this;
    ++*this;
    return this_copy;
}

template<typename tkey, typename tvalue>
bool b_tree<tkey, tvalue>::iterator::operator==(iterator const &other) const
{
    if (_path.empty() && other._path.empty())
        return true;

    if (_path.empty() || other._path.empty())
        return false;

    return _path.top().first == other._path.top().first && _item_index == other._item_index;
}

template<typename tkey, typename tvalue>
bool b_tree<tkey, tvalue>::iterator::operator!=(iterator const &other) const
{
    return !(*this == other);
}

template<typename tkey, typename tvalue>
std::tuple<tkey, tvalue &, size_t, size_t> b_tree<tkey, tvalue>::iterator::operator*()
{

    if (_path.empty())
    {
        throw std::out_of_range("unmock end iterator");
    }

    //TODO: Whats the values
     return std::tuple<tkey, tvalue &, size_t, size_t>(
            _path.top().first->items[_item_index].key,
            _path.top().first->items[_item_index].value,
            0,
            0);
}

template<typename tkey, typename tvalue>
void b_tree<tkey, tvalue>::iterator::fall_to_right_subtree()
{
    _path.top().second = -(_item_index+1)-1;
    _path.push(std::pair(
            _path.top().first->subtrees[_item_index+1],
            -1));
    _item_index = 0;

    while (_path.top().first->subtrees[0]!=nullptr)
    {
        _path.push(
            std::pair(_path.top().first->subtrees[0], -1));
    }

}

template<typename tkey, typename tvalue>
void b_tree<tkey, tvalue>::iterator::rise_to_first_unfinished()
{
    while (!_path.empty())
    {
        _path.pop();

        if (_path.empty())
            return;

        _item_index =
           _path.top().second > 0 ?
           _path.top().second :
           -(_path.top().second)-1;

        if (_item_index < _path.top().first->virtual_size)
        {
            return;
        }

    }
}
