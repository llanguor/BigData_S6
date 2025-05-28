#pragma once
#include <algorithm>
#include <functional>
#include <iostream>
#include <memory>
#include <stack>
#include <string>
#include <vector>

#include "hasher.hpp"
#include "../hasher_adapter/include/hasher_adapter.hpp"

class bloom_filter final
{

private:

   unsigned long long _size;
    std::unique_ptr<bool[]> _bits;
    std::vector<hasher_adapter> _hashes;

public:

    explicit bloom_filter(
        std::vector<hasher_adapter> & hashes,
        const unsigned long long & size):
        _size(size),
        _bits(std::make_unique<bool[]>(size)),
        _hashes(std::move(hashes))
        {
        }

public:

    void append(std::string const & value)
    {
        auto result = _hashes.at(0).get_hash_code(value, _size);
        _bits[result] = true;

        for (int i = 1; i < _hashes.size(); ++i)
        {
            result = _hashes.at(i).get_hash_code(result, _size);;
            _bits[result] = true;
        }
    }

    bool find(std::string const & value)
    {
        bool is_value_exists = true;

        auto result = _hashes.at(0).get_hash_code(value, _size);
        is_value_exists = is_value_exists && _bits[result];

        for (int i = 1; i < _hashes.size(); ++i)
        {
            result = _hashes.at(i).get_hash_code(result, _size);;
            is_value_exists = is_value_exists && _bits[result];
        }

        return is_value_exists;
    }
};
