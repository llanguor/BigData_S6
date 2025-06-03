#include <iostream>
#include "hash_table.hpp"
#include "hash_provider_bytes_adapter.hpp"
#include "hash_provider_md5.hpp"
#include "hash_provider_sha256.hpp"
#include "hash_provider_sha512.hpp"

bool is_equals(std::string const & s1, std::string const & s2)
{
    return s1==s2;
}


int main()
{
    hash_provider_bytes_adapter<std::string> adapter_sha256(std::make_shared<hash_provider_sha256<std::string>>());
    hash_provider_bytes_adapter<std::string> adapter_sha512(std::make_shared<hash_provider_sha256<std::string>>());

    auto table = hash_table<std::string, int>::create_with_cuckoo(
        std::move(adapter_sha512),
        std::move(adapter_sha256),
        is_equals,
        1
        );

    table.insert("key0", 0);
    table.dispose("key0");
    try
    {
        std::cout << table.obtain("key0") << std::endl;
    }
    catch (const std::exception & e)
    {
        std::cout << e.what() << std::endl;
    }

    table.insert("key0", 0);
    std::cout << table.obtain("key0") << std::endl;


    for (int i = 1; i < 27; ++i)
    {
        table.insert("key" + std::to_string(i), i);
    }

    for (int i = 1; i < 27; ++i)
    {
        std::cout << table.obtain("key" + std::to_string(i)) << std::endl;
    }
}