#pragma once
#include <algorithm>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "hasher.hpp"

template<typename hasher_type>
    class bloom_filter final
{

private:

     std::unique_ptr<hasher<
        typename hasher_type::input_type,
         hasher_type::hash_size>> _hasher;

public:

    explicit bloom_filter():
    _hasher(std::make_unique<hasher_type>())
    {
        std::cout<< _hasher->hash_size <<std::endl;
    }
};
