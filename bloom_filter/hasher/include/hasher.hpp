#pragma once

template<
    typename T>
class hasher
{
public:

    virtual int get_hash_code() = 0;

    virtual ~hasher() = default;

};