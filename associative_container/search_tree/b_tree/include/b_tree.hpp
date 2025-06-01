#pragma once
#include <functional>
#include <memory>
#include <stack>
#include "search_tree.hpp"

template<typename tkey, typename tvalue>
class b_tree final:
    public search_tree<tkey, tvalue>
{
public:

    class iterator;

    class iterator_const;

public:

    class node final
    {
    public:

        typename associative_container<tkey,
            tvalue>::key_value_pair * items;

        node ** subtrees;

        size_t virtual_size;

    public:

        explicit node(
            typename associative_container<tkey,tvalue>::key_value_pair * items,
            node ** subtrees):
            items(items),
            subtrees(subtrees),
            virtual_size(0)
        {}

        explicit node(size_t const size):
            virtual_size(0)
        {
            items = new typename associative_container<tkey, tvalue>::key_value_pair[size];
            subtrees = new node *[size+1]();
        }

        node(const node & other) = delete;
        node & operator=(const node & other) = delete;
        node(node && other) noexcept = delete;
        node & operator=(node && other) noexcept = delete;

        ~node()
        {
            delete[] items;
            delete[] subtrees;
        }
    };

private:

    node * _root;
    size_t _elements_count = 0;
    std::stack<std::pair<node**,int>> _stack;

    size_t _degree;
    std::function<int(tkey const &, tkey const &)> _comparer;


public:

    [[nodiscard]] size_t elements_count() const override
    {
        return _elements_count;
    }

public:

    explicit b_tree(
        size_t t,
        std::function<int(tkey const &, tkey const &)> comparer);

private:

    int get_real_index(int index);

    void obtain_node(
        tkey const &key);

    int dichotomous_upper_bound_search(
        typename associative_container<tkey,tvalue>::key_value_pair * array,
        int array_size,
        tkey const &key);

    void upsert(
        tkey const &key,
        tvalue &&value,
        bool is_only_update_operation = false);

    void insert_left(
        tkey &insert_key,
        tkey &insert_value,
        node *&insert_left_node,
        node *&insert_right_node,
        node *&current_node,
        int & middle_index,
        int & insert_index);

    void insert_right(
        tkey &insert_key,
        tkey &insert_value,
        node *&insert_left_node,
        node *&insert_right_node,
        node *&current_node,
        int & middle_index,
        int & insert_index);

    void insert_middle(
        node *&insert_left_node,
        node *&insert_right_node,
        node *&current_node,
        int & middle_index);

    iterator obtain_iterator_near_the_bound(
        tkey const &bound);

public:

    void insert(
        tkey const &key,
        tvalue const &value) override;

    void insert(
        tkey const &key,
        tvalue &&value) override;

    void update(
        tkey const &key,
        tvalue const &value) override;

    void update(
        tkey const &key,
        tvalue &&value) override;

    tvalue &obtain(
        tkey const &key) override;

    tvalue dispose(
        tkey const &key) override;

    std::vector<typename associative_container<tkey, tvalue>::key_value_pair> obtain_between(
        tkey const &lower_bound,
        tkey const &upper_bound,
        bool lower_bound_inclusive,
        bool upper_bound_inclusive) override;

public:

    std::ranges::subrange<typename b_tree<tkey, tvalue>::iterator> obtain_between_range(
        tkey const &lower_bound,
        tkey const &upper_bound,
        bool lower_bound_inclusive,
        bool upper_bound_inclusive);


public:

    iterator begin() const;

    iterator end() const;

    iterator_const cbegin() const;

    iterator_const cend() const;

public:

    b_tree(
        b_tree const &other) = delete;

    b_tree &operator=(
        b_tree const &other) = delete;

    b_tree (
        b_tree && other) noexcept;

    b_tree &operator=(
        b_tree && other) noexcept;

    ~b_tree() noexcept;

private:

    void release() noexcept;

    void move_from(b_tree&& other) noexcept;

};

#include "b_tree.tpp"
