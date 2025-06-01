#pragma once
#include <format>
#include <ranges>
#include "b_tree.hpp"
#include "iterator.hpp"

#pragma region Constructors

template<typename tkey, typename tvalue>
b_tree<tkey, tvalue>::b_tree(
    const size_t t,
    std::function<int(tkey const &, tkey const &)> comparer):
    _root(nullptr),
    _elements_count(0),
    _comparer(comparer)
{
    if (t<2)
    {
        throw std::invalid_argument("Degree must be more than 1");
    }

    _degree = t;
}

template<typename tkey, typename tvalue>
int b_tree<tkey, tvalue>::get_real_index(int index)
{
    if (index<0)
        return -index-1;

    return index;
}

#pragma endregion

#pragma region tree public methods

template<typename tkey, typename tvalue>
void b_tree<tkey, tvalue>::insert(tkey const &key, tvalue const &value)
{
    insert(key, tvalue(value));
}

template<typename tkey, typename tvalue>
void b_tree<tkey, tvalue>::insert(tkey const &key, tvalue &&value)
{
    upsert(key, std::move(value));
}

template<typename tkey, typename tvalue>
void b_tree<tkey, tvalue>::update(tkey const &key, tvalue const &value)
{
    update(key, tvalue(value));
}

template<typename tkey, typename tvalue>
void b_tree<tkey, tvalue>::update(tkey const &key, tvalue &&value)
{
    upsert(key, std::move(value), true);
}


template<typename tkey, typename tvalue>
tvalue & b_tree<tkey, tvalue>::obtain(tkey const &key)
{
    obtain_node(key);

    if (_stack.empty() || _stack.top().second < 0)
    {
        throw std::out_of_range(
            std::format("b_tree::obtain: key {} not found", key));
    }

    return (*_stack.top().first)->items[_stack.top().second].value;
}

template<typename tkey, typename tvalue>
tvalue b_tree<tkey, tvalue>::dispose(tkey const &key)
{

    obtain(key);

    if (_stack.empty() || _stack.top().second < 0)
    {
       /* throw std::out_of_range(
            std::format("key {} doesn't exists", key)); */
        return tvalue{};
    }

    auto node_to_delete = *_stack.top().first;
    auto index_to_delete = _stack.top().second;
    auto value_to_delete = std::move(node_to_delete->items[index_to_delete].value);

    //delete node in middle. swap key and value with infix-previous node
    if (index_to_delete >= 0 &&
        node_to_delete->subtrees[0]!=nullptr)
    {
        _stack.push(std::pair(
                &node_to_delete->subtrees[index_to_delete],
                node_to_delete->subtrees[index_to_delete]->virtual_size
                ));

        while ((*_stack.top().first)->subtrees[0]!=nullptr)
        {
            auto last_subtree_index = (*_stack.top().first)->virtual_size;

            _stack.push(std::pair(
                &(*_stack.top().first)->subtrees[last_subtree_index],
                last_subtree_index
                ));
        }

        auto swap_node = *_stack.top().first;
        auto swap_index = swap_node->virtual_size-1;

        std::swap(
            swap_node->items[swap_index].key,
            node_to_delete->items[index_to_delete].key
        );

        std::swap(
            swap_node->items[swap_index].value,
            node_to_delete->items[index_to_delete].value
        );

        node_to_delete = *_stack.top().first;
        index_to_delete = _stack.top().second;
    }

    //delete key inside last node, if (node is not root and size > t-1) or (node is root and size > 1)
    if (node_to_delete->virtual_size > _degree-1 ||
        node_to_delete==_root &&
        node_to_delete->virtual_size > 1)
    {
        for (auto i = index_to_delete; i < node_to_delete->virtual_size-1; ++i)
        {
            node_to_delete->items[i] = node_to_delete->items[i+1];
            node_to_delete->subtrees[i+1] = node_to_delete->subtrees[i+2];
        }

        --node_to_delete->virtual_size;
        --_elements_count;
        return value_to_delete;
    }

    //delete if size == 1 and node is _root
    if (node_to_delete==_root)
    {
        delete _root;
        _root = nullptr;
        _elements_count = 0;
        return value_to_delete;
    }


    //delete if size <= t-1 and node not is _root
    _stack.pop();
    auto parent_node = *_stack.top().first;
    auto parent_separate_index = get_real_index(_stack.top().second) - 1;

    node * left_son = nullptr;
    if (parent_separate_index>=0)
    {
        left_son = parent_node->subtrees[parent_separate_index];
    }
    node *  right_son = nullptr;
    if (parent_separate_index+2 <= parent_node->virtual_size)
    {
        right_son = parent_node->subtrees[parent_separate_index+2];
    }

    if (left_son!=nullptr &&
        left_son->virtual_size > _degree-1)
    {
        node_to_delete->items[index_to_delete] =
            std::move(parent_node->items[parent_separate_index]);

        parent_node->items[parent_separate_index] =
            std::move(left_son->items[left_son->virtual_size-1]);

        --left_son->virtual_size;
        --_elements_count;
        return value_to_delete;
    }
    else if (right_son!=nullptr &&
        right_son->virtual_size > _degree-1)
    {
        node_to_delete->items[index_to_delete] =
            std::move(parent_node->items[parent_separate_index+1]);

        parent_node->items[parent_separate_index+1] =
            std::move(right_son->items[0]);

        for (int i=0; i < right_son->virtual_size-1; ++i)
        {
            right_son->items[i] = std::move(right_son->items[i+1]);
        }

        --right_son->virtual_size;
        --_elements_count;
        return value_to_delete;
    }


    //Поменять условие
    //ОДАЛЖИВАНИЕ ТОЖЕ НАДО??? ПЕРЕСМОТРЕТЬ

    while (node_to_delete!=_root)
    {

        //can't borrow from sons
        if (left_son!=nullptr)
        {
            //move parents key to left son
            left_son->items[left_son->virtual_size] =
                std::move(parent_node->items[parent_separate_index]);
            ++left_son->virtual_size;

            for (int i = parent_separate_index; i < parent_node->virtual_size-1; ++i)
            {
                parent_node->items[i] = std::move(parent_node->items[i+1]);
                parent_node->subtrees[i+1] = parent_node->subtrees[i+2];
            }

            --parent_node->virtual_size;

            parent_node->subtrees[parent_separate_index] = left_son;

            //move current node keys to left son
            if (node_to_delete->virtual_size > 0)
            {
                for (int i=0; i < node_to_delete->virtual_size; ++i)
                {
                    if (i!=index_to_delete)
                    {
                        left_son->items[left_son->virtual_size] =
                            std::move(node_to_delete->items[i]);
                        left_son->subtrees[left_son->virtual_size] = node_to_delete->subtrees[i];
                        ++left_son->virtual_size;
                    }
                }
            }
            else
            {
                left_son->subtrees[left_son->virtual_size] = node_to_delete->subtrees[0];
            }

            //delete node_to_delete
            delete node_to_delete;
        }
        else if (right_son!=nullptr)
        {
            //delete key from current node
            if (node_to_delete->virtual_size > 0)
            {
                for (int i = index_to_delete; i < node_to_delete->virtual_size-1; ++i)
                {
                    node_to_delete->items[i] = std::move(node_to_delete->items[i+1]);
                    node_to_delete->subtrees[i+1] = node_to_delete->subtrees[i+2];
                }

                --node_to_delete->virtual_size;
            }

            //move parents key to current node
            node_to_delete->items[node_to_delete->virtual_size] =
                std::move(parent_node->items[parent_separate_index+1]);

            ++node_to_delete->virtual_size;

            for (int i = parent_separate_index+1; i < parent_node->virtual_size-1; ++i)
            {
                parent_node->items[i] = std::move(parent_node->items[i+1]);
                parent_node->subtrees[i+1] = parent_node->subtrees[i+2];
            }

            --parent_node->virtual_size;

            parent_node->subtrees[parent_separate_index+1] = node_to_delete;

            //move right son's keys to current node
            for (int i=0; i < right_son->virtual_size; ++i)
            {
                node_to_delete->items[node_to_delete->virtual_size] =
                    std::move(right_son->items[i]);
                node_to_delete->subtrees[node_to_delete->virtual_size] = right_son->subtrees[i];
                ++node_to_delete->virtual_size;
            }

            node_to_delete->subtrees[node_to_delete->virtual_size] = right_son->subtrees[right_son->virtual_size];

            //delete right son
            delete right_son;
        }


        if (parent_node == _root && parent_node->virtual_size==0)
        {
            auto node = parent_node->subtrees[0];
            delete _root;
            _root = node;
            --_elements_count;
            return value_to_delete;
        }

        if (parent_node->virtual_size >= _degree-1)
        {
            --_elements_count;
            return value_to_delete;
        }
        else
        {
            node_to_delete = parent_node;
            _stack.pop();
            parent_node = *_stack.top().first;
            //
            parent_separate_index = get_real_index(_stack.top().second) - 1;

            left_son = nullptr;
            if (parent_separate_index>=0)
            {
                left_son = parent_node->subtrees[parent_separate_index];
            }
            right_son = nullptr;
            if (parent_separate_index+2 <= parent_node->virtual_size)
            {
                right_son = parent_node->subtrees[parent_separate_index+2];
            }
        }
    }

    --_elements_count;
    return value_to_delete;

}

template<typename tkey, typename tvalue>
std::vector<typename associative_container<tkey, tvalue>::key_value_pair> b_tree<tkey, tvalue>::obtain_between(
    tkey const &lower_bound, tkey const &upper_bound, bool lower_bound_inclusive, bool upper_bound_inclusive)
{


    auto range = obtain_between_range(
        lower_bound,
        upper_bound,
        lower_bound_inclusive,
        upper_bound_inclusive);

    auto pairs = std::vector<typename associative_container<tkey, tvalue>::key_value_pair>();


    for (auto&& pair : range)
    {
        auto [key, value, v1, v2] = pair;
        pairs.emplace_back(key, value);
    }


    return pairs;
}

template<typename tkey, typename tvalue>
std::ranges::subrange<typename ::b_tree<tkey, tvalue>::iterator> b_tree<tkey, tvalue>::obtain_between_range(
    tkey const &lower_bound, tkey const &upper_bound,
    bool lower_bound_inclusive, bool upper_bound_inclusive)
{
    auto begin_it = obtain_iterator_near_the_bound(lower_bound);
    if (begin_it != this->end())
    {
        auto [key, value, v1, v2] = *begin_it;
        if (_comparer(key, lower_bound) < 0 ||
           _comparer(key, lower_bound) == 0 &&
            !lower_bound_inclusive)
        {
            ++begin_it;
        }
    }

    auto end_it = obtain_iterator_near_the_bound(upper_bound);
    if (end_it != this->end())
    {
        auto [key, value, v1, v2] = *end_it;
        if (_comparer(key, upper_bound) == 0 &&
            upper_bound_inclusive)
        {
            ++end_it;
        }
    }

    return std::ranges::subrange(begin_it, end_it);

}

#pragma endregion

#pragma region tree private methods

template<typename tkey, typename tvalue>
void b_tree<tkey, tvalue>::obtain_node(tkey const &key)
{
    _stack = {};

    auto ** current = &_root;

    while (*current != nullptr)
    {

        auto v = (*current)->virtual_size;

        auto i = dichotomous_upper_bound_search(
            (*current)->items,
            (*current)->virtual_size,
            key);

        _stack.emplace(current, i);

        if (i >= 0)
        {
            return;
        }
        else
        {
            current = &(*current)->subtrees[-i-1];
        }
    }
}

template<typename tkey, typename tvalue>
int b_tree<tkey, tvalue>::dichotomous_upper_bound_search(
    typename associative_container<tkey, tvalue>::key_value_pair * array,
    const int array_size,
    tkey const &key)
{
    int start_index = 0;
    int end_index = array_size;

    while (start_index < end_index)
    {
        int middle_index = start_index + (end_index - start_index) / 2;

        if (_comparer(key, array[middle_index].key) > 0)
        {
            start_index = middle_index + 1;
        }
        else
        {
            end_index = middle_index;
        }
    }

    if (start_index < array_size && _comparer(array[start_index].key, key)==0)
    {
        return start_index;
    }
    else
    {
        return -start_index-1;
    }
}


template<typename tkey, typename tvalue>
void b_tree<tkey, tvalue>::upsert(
    tkey const &key,
    tvalue &&value,
    bool is_only_update_operation)
{
    //if tree is empty
    if (_root == nullptr)
    {
        _root = new node(_degree*2-1);
        _root->items[_root->virtual_size].value = std::move(value);
        _root->items[_root->virtual_size].key = std::move(key);
        ++_root->virtual_size;
        ++_elements_count;
        return;
    }

    obtain_node(key);

    //if key already exists
    if (_stack.top().second >= 0)
    {
        (*_stack.top().first)->items[_stack.top().second].value = std::move(value);
        return;
    }
    else if (is_only_update_operation)
    {
        throw std::runtime_error("upsert operation: key "+ std::to_string(key) +" already exists");
    }

    //if key not exists
    tkey insert_key = std::move(key);
    tvalue insert_value = std::move(value);
    node * insert_left_node = nullptr;
    node * insert_right_node = nullptr;

    while (!_stack.empty())
    {
        auto current_node = *_stack.top().first;
        auto insert_index = -_stack.top().second-1;
        _stack.pop();

        //if there is still space in the array
        if (current_node->virtual_size < _degree*2-1)
        {
            for (int i = current_node->virtual_size; i > insert_index; --i)
            {
                current_node->items[i] = std::move(current_node->items[i-1]);
                current_node->subtrees[i+1] = current_node->subtrees[i];
            }

            current_node->items[insert_index].key = std::move(insert_key);
            current_node->items[insert_index].value = std::move(insert_value);
            current_node->subtrees[insert_index] = insert_left_node;
            current_node->subtrees[insert_index+1] = insert_right_node;
            ++current_node->virtual_size;
            ++_elements_count;
            return;
        }

        //no more space in array
        int middle_index = current_node->virtual_size / 2;
        if (insert_index == middle_index)
        {
            insert_middle(
                insert_left_node,
                insert_right_node,
                current_node,
                middle_index);
        }
        else if (insert_index < middle_index)
        {
            insert_left(
                insert_key,
                insert_value,
                insert_left_node,
                insert_right_node,
                current_node,
                middle_index,
                insert_index);
        }
        else
        {
            insert_right(
                insert_key,
                insert_value,
                insert_left_node,
                insert_right_node,
                current_node,
                middle_index,
                insert_index);

        }
    }

    ++_elements_count;
    _root = new node(_degree*2-1);
    _root->virtual_size = 1;
    _root->items[0].key = std::move(insert_key);
    _root->items[0].value = std::move(insert_value);
    _root->subtrees[0] = std::move(insert_left_node);
    _root->subtrees[1] = std::move(insert_right_node);
}

template<typename tkey, typename tvalue>
void b_tree<tkey, tvalue>::insert_left(
    tkey &insert_key,
    tkey &insert_value,
    node *&insert_left_node,
    node *&insert_right_node,
    node *&current_node,
    int & middle_index,
    int & insert_index)
{
    --middle_index;
    tkey middle_key = std::move(current_node->items[middle_index].key);
    tvalue middle_value = std::move(current_node->items[middle_index].value);
    node * middle_left_node = current_node;
    node * middle_right_node =  new node(_degree*2-1);

    middle_right_node->subtrees[0] = std::move(current_node->subtrees[middle_index+1]);
    for (int i = middle_index+1; i < current_node->virtual_size; ++i)
    {
        middle_right_node->items[middle_right_node->virtual_size] = std::move(current_node->items[i]);
        middle_right_node->subtrees[middle_right_node->virtual_size+1] = std::move(current_node->subtrees[i+1]);
        ++middle_right_node->virtual_size;
    }

    current_node->virtual_size = middle_index + 1;

    for (int i=middle_index; i>insert_index; --i)
    {
        current_node->items[i] = std::move(current_node->items[i-1]);
        current_node->subtrees[i+1] = std::move(current_node->subtrees[i]);
    }

    current_node->items[insert_index].key = std::move(insert_key);
    current_node->items[insert_index].value = std::move(insert_value);
    current_node->subtrees[insert_index] = insert_left_node;
    current_node->subtrees[insert_index+1] = insert_right_node;

    insert_key = std::move(middle_key);
    insert_value = std::move(middle_value);
    insert_left_node = middle_left_node;
    insert_right_node = middle_right_node;
}

template<typename tkey, typename tvalue>
void b_tree<tkey, tvalue>::insert_right(
    tkey &insert_key,
    tkey &insert_value,
    node *&insert_left_node,
    node *&insert_right_node,
    node *&current_node,
    int & middle_index,
    int & insert_index)
{
    tkey middle_key = std::move(current_node->items[middle_index].key);
    tvalue middle_value = std::move(current_node->items[middle_index].value);
    node * middle_left_node = current_node;
    node * middle_right_node =  new node(_degree*2-1);

    middle_right_node->subtrees[0] = std::move(current_node->subtrees[middle_index+1]);
    for (int i = middle_index+1; i <= current_node->virtual_size; ++i)
    {
        if (insert_index == i)
        {
            middle_right_node->items[middle_right_node->virtual_size].key = std::move(insert_key);
            middle_right_node->items[middle_right_node->virtual_size].value = std::move(insert_value);
            middle_right_node->subtrees[middle_right_node->virtual_size] = insert_left_node;
            middle_right_node->subtrees[middle_right_node->virtual_size+1] = insert_right_node;
            ++middle_right_node->virtual_size;
        }

        if (i < current_node->virtual_size)
        {
            middle_right_node->items[middle_right_node->virtual_size] = std::move(current_node->items[i]);
            middle_right_node->subtrees[middle_right_node->virtual_size+1] = std::move(current_node->subtrees[i+1]);
            ++middle_right_node->virtual_size;
        }
    }

    current_node->virtual_size = middle_index;

    insert_key = std::move(middle_key);
    insert_value = std::move(middle_value);
    insert_left_node = middle_left_node;
    insert_right_node = middle_right_node;
}

template<typename tkey, typename tvalue>
void b_tree<tkey, tvalue>::insert_middle(
    node *&insert_left_node,
    node *&insert_right_node,
    node *&current_node,
    int & middle_index)
{
    node * middle_left_node = current_node;
    node * middle_right_node =  new node(_degree*2-1);
    middle_right_node->subtrees[0] = std::move(insert_right_node);
    for (int i = middle_index; i<current_node->virtual_size; ++i)
    {
        middle_right_node->items[middle_right_node->virtual_size] = std::move(current_node->items[i]);
        middle_right_node->subtrees[middle_right_node->virtual_size+1] = std::move(current_node->subtrees[i+1]);
        ++middle_right_node->virtual_size;
    }

    current_node->virtual_size = middle_index;
    current_node->subtrees[middle_index] = insert_left_node;

    insert_left_node = middle_left_node;
    insert_right_node = middle_right_node;
}

template<typename tkey, typename tvalue>
typename b_tree<tkey, tvalue>::iterator b_tree<tkey, tvalue>::obtain_iterator_near_the_bound(
    tkey const &bound)
{
    std::stack<std::pair<node*,int>> path;
    auto current = _root;

    while (current != nullptr)
    {
        //find first upper bound item
        auto i = dichotomous_upper_bound_search(
           current->items,
           current->virtual_size,
           bound);

        //go to the left subtree
        path.push(std::pair(current, i));
        if (i < 0)
        {
            i = -i-1;
        }
        else
        {
            break;
        }

        current = current->subtrees[i];
    }

    int current_item_index =
        path.top().second < 0?
        -path.top().second-1 :
        path.top().second;

    if (current_item_index == path.top().first->virtual_size)
    {
        path.top().second < 0 ? ++path.top().second : --path.top().second;
    }

    return iterator(path);
}


#pragma endregion

#pragma region iterators

template<typename tkey, typename tvalue>
typename b_tree<tkey, tvalue>::iterator b_tree<tkey, tvalue>::begin() const
{
    std::stack<std::pair<node*,int>> path;
    path.push(std::pair(_root,-1));
    while (path.top().first->subtrees[0]!=nullptr)
    {
        path.push(std::pair(path.top().first->subtrees[0], -1));
    }

    return iterator(path);
}

template<typename tkey, typename tvalue>
typename b_tree<tkey, tvalue>::iterator b_tree<tkey, tvalue>::end() const
{
    return iterator( std::stack<std::pair<node*,int>>());
}

template<typename tkey, typename tvalue>
typename b_tree<tkey, tvalue>::iterator_const b_tree<tkey, tvalue>::cbegin() const
{
    std::stack<std::pair<node*,int>> path;
    path.push(std::pair(_root,-1));
    while (path.top().first->subtrees[0]!=nullptr)
    {
        path.push(std::pair(path.top().first->subtrees[0], -1));
    }

    return iterator_const(path);
}

template<typename tkey, typename tvalue>
typename b_tree<tkey, tvalue>::iterator_const b_tree<tkey, tvalue>::cend() const
{
    return iterator_const( std::stack<std::pair<node*,int>>());
}

#pragma endregion

#pragma region rool of five

template<typename tkey, typename tvalue>
    b_tree<tkey, tvalue>::b_tree(b_tree &&other) noexcept
{
    move_from(std::move(other));
}

template<typename tkey, typename tvalue>
    b_tree<tkey, tvalue> & b_tree<tkey, tvalue>::operator=(b_tree &&other) noexcept
{
    if (this != &other)
    {
        release();
        move_from(std::move(other));
    }

    return *this;
}

template<typename tkey, typename tvalue>
    b_tree<tkey, tvalue>::~b_tree() noexcept
{
    release();
}

template<typename tkey, typename tvalue>
    void  b_tree<tkey, tvalue>::release() noexcept
{
    if (_root == nullptr)
        return;

    _stack = {};
    _stack.push( {&_root, 0});

    while (!_stack.empty())
    {
        int index = _stack.top().second;

        if ((*_stack.top().first)->subtrees[index]!=nullptr &&
            index <= (*_stack.top().first)->virtual_size)
        {
            _stack.push({&(*_stack.top().first)->subtrees[index], 0});

            while ((*_stack.top().first)->subtrees[0]!=nullptr)
            {
                _stack.push({&(*_stack.top().first)->subtrees[0], 0});
            }

        }

        delete *_stack.top().first;
        _stack.pop();
        if (!_stack.empty())
        {
            ++_stack.top().second;
        }

    }

    _root = nullptr;
    _elements_count = 0;
}

template<typename tkey, typename tvalue>
    void b_tree<tkey, tvalue>::move_from(b_tree&& other) noexcept
{
    _degree = other._degree;
    _elements_count = other._elements_count;
    _comparer = std::move(other._comparer);
    _root = std::move(other._root);
    _stack = std::move(other._stack);

    other._elements_count = 0;
    other._comparer = nullptr;
    other._root = nullptr;
}


#pragma endregion
