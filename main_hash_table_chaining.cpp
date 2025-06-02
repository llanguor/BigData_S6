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

    auto table = hash_table<std::string, int>::сreate_with_chaining(
        std::move(adapter_sha256),
        is_equals,
        32
        );

    table.insert("key1", 10);
    table.insert("key2", 20);
    table.insert("key3", 30);
    std::cout << table.obtain("key1") << std::endl;
    std::cout << table.obtain("key2") << std::endl;

    table.dispose("key3");
    try
    {
        std::cout << table.obtain("key3") << std::endl;
    }
    catch (const std::exception & e)
    {
        std::cout << e.what() << std::endl;
    }

    table.insert("key3", 30);
    std::cout << table.obtain("key3") << std::endl;

}