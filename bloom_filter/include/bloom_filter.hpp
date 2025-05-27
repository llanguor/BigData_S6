#pragma once
#include <algorithm>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "hasher.hpp"

class bloom_filter final
{

private:

    std::unique_ptr<bool[]> _bits;
    std::function<size_t(std::string const &)> _adapter_hash_function;


public:

    explicit bloom_filter(
        std::function<size_t(std::string const &)> & adapter_hash_function,
        size_t size);
};
