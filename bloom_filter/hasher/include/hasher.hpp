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

protected:

    virtual unsigned char* get_hash_raw(const void* data, size_t size) = 0;

public:

    virtual unsigned char* get_hash_code(const std::string& input)
    {
        return get_hash_raw(input.data(), input.size());
    }

    virtual unsigned char* get_hash_code(const unsigned long long& input)
    {
        return get_hash_raw(&input, sizeof(input));
    }

public:

    virtual ~hasher() = default;
};
