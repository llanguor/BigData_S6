#pragma once


class hasher
{
private:

    size_t const _hash_size;

protected:

    std::shared_ptr<unsigned char[]> _bufer = nullptr;

public:

    explicit hasher(const size_t hash_size):
        _hash_size(hash_size)
    {
        _bufer = std::make_shared<unsigned char[]>(_hash_size);
    }

public:

    [[nodiscard]] size_t get_hash_size() const
    {
        return _hash_size;
    }

public:

    //unsigned char for storage bytes
    virtual unsigned char * get_hash_code(std::string const & input) = 0;

public:

    virtual ~hasher() = default;
};
