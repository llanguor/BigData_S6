#include <array>
#include <iomanip>
#include <iostream>
#include "bloom_filter.hpp"
#include <openssl/md5.h>

#include "hasher_md5.hpp"


int main() {
    std::string input = "hello world";


    std::stack<hasher_adapter<hasher>> stack { hasher_adapter<hasher_md5>() };
    //bloom_filter<hasher_md5> bf;

    return 0;
}
