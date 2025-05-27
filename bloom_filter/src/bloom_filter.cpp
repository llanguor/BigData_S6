#include "bloom_filter.hpp"

bloom_filter::bloom_filter(
    std::function<size_t(std::string const &)> & adapter_hash_function,
    size_t const size):
    _adapter_hash_function(std::move(adapter_hash_function)),
    _bits(std::make_unique<bool[]>(size))
{
}
