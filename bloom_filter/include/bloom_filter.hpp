#pragma once
#include <algorithm>
#include <functional>
#include <iostream>
#include <memory>
#include <stack>
#include <string>
#include <vector>

#include "hasher.hpp"
#include "../hasher/hasher_numeric/include/hasher_numberic.hpp"


template<typename datatype>
class bloom_filter final
{

private:

   unsigned long long _size;
    std::unique_ptr<bool[]> _bits;
    std::vector<hasher_numeric<datatype> *> _hashes;

public:

    explicit bloom_filter(
        std::vector<hasher_numeric<datatype> *> & hashes,
        const unsigned long long & size):
        _size(size),
        _bits(std::make_unique<bool[]>(size)),
        _hashes(std::move(hashes))
        {
        }

public:

    void append(datatype const & value)
    {
        auto result = _hashes.at(0)->get_hash_code(value);
        _bits[result] = true;

        for (int i = 1; i < _hashes.size(); ++i)
        {
            result = _hashes.at(i)->get_hash_code(result);;
            _bits[result] = true;
        }
    }

    bool find(datatype const & value)
    {
        auto result = _hashes.at(0)->get_hash_code(value);

        for (int i = 1; i < _hashes.size(); ++i)
        {
            result = _hashes.at(i)->get_hash_code(result);;
            if (!_bits[result])
            {
                return false;
            }
        }

        return true;
    }
};
