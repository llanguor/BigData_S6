#include <array>
#include <cmath>
#include <iomanip>
#include <iostream>
#include "bloom_filter.hpp"
#include <openssl/md5.h>
#include "hasher_md5.hpp"
#include "hasher_sha256.hpp"
#include "hasher_sha512.hpp"


int main() {
    std::string input = "hello world";

    auto md5 = std::make_shared<hasher_md5<std::string>>();
    auto sha256 = std::make_shared<hasher_sha256<std::string>>();
    auto sha512 = std::make_shared<hasher_sha512<std::string>>();

    hasher_adapter<std::string> adapter_md5(md5);
    hasher_adapter<std::string> adapter_sha256(sha256);
    hasher_adapter<std::string> adapter_sha512(sha512);

    std::vector<hasher_adapter<std::string>> vector;
    vector.push_back(adapter_md5);
    vector.push_back(adapter_sha256);
    vector.push_back(adapter_sha512);


    bloom_filter bf (vector, 8);
    bf.append("asd");
    bf.append("test");

    std::cout << bf.find("asd") << std::endl;
    std::cout << bf.find("test") << std::endl;
    std::cout << bf.find("qwerty") << std::endl;
    return 0;
}
