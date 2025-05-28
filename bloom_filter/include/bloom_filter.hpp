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

    std::unique_ptr<bool[]> _bits;
    std::vector<hasher_adapter<hasher>> _hashes;

public:

    explicit bloom_filter(
        std::vector<hasher_adapter<hasher>> const & hashes,
        size_t const size):
        _bits(std::make_unique<bool[]>(size)),
        _hashes(hashes)
        {
        }

};
