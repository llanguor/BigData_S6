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

int main() {
    std::string input = "hello world";

    auto md5 = std::make_shared<hash_provider_md5<std::string>>();
    auto sha256 = std::make_shared<hash_provider_sha256<std::string>>();
    auto sha512 = std::make_shared<hash_provider_sha512<std::string>>();

    hash_provider_bytes_adapter<std::string> adapter_md5(md5);
    hash_provider_bytes_adapter<std::string> adapter_sha256(sha256);
    hash_provider_bytes_adapter<std::string> adapter_sha512(sha512);

    std::vector<hash_provider_numeric<std::string> *> vector;
    vector.emplace_back(&adapter_md5);
    vector.emplace_back(&adapter_sha256);
    vector.emplace_back(&adapter_sha512);


    bloom_filter<std::string> bf (vector, 32);
    bf.append("asd");
    bf.append("test");

    std::cout << bf.find("asd") << std::endl;
    std::cout << bf.find("test") << std::endl;
    std::cout << bf.find("qwerty") << std::endl;
    return 0;
}
