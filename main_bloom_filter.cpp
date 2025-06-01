#include <array>
#include <cmath>
#include <iomanip>
#include <iostream>
#include "bloom_filter.hpp"
#include <openssl/md5.h>
#include "hash_provider_bytes_adapter.hpp"
#include "hash_provider_md5.hpp"
#include "hash_provider_sha256.hpp"
#include "hash_provider_sha512.hpp"

int main()
{
    hash_provider_bytes_adapter<std::string> adapter_md5(std::make_unique<hash_provider_md5<std::string>>());
    hash_provider_bytes_adapter<std::string> adapter_sha256(std::make_unique<hash_provider_sha256<std::string>>());
    hash_provider_bytes_adapter<std::string> adapter_sha512(std::make_unique<hash_provider_sha512<std::string>>());

    std::vector<hash_provider_numeric<std::string> *> vector;
    vector.push_back(&adapter_md5);
    vector.push_back(&adapter_sha256);
    vector.push_back(&adapter_sha512);


    bloom_filter<std::string> bf (vector, 32);
    bf.append("asd");
    bf.append("test");

    std::cout << bf.find("asd") << std::endl;
    std::cout << bf.find("test") << std::endl;
    std::cout << bf.find("qwerty") << std::endl;
    return 0;
}
