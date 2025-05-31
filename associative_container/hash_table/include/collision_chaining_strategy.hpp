#pragma once
#include "collision_strategy.hpp"
#include "key_value_pair.hpp"


template <
    typename tkey,
    typename tvalue>
class collision_chaining_strategy final : public collision_strategy<tkey, tvalue>
{
public:
    collision_chaining_strategy();
};
