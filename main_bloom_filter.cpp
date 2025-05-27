#include <array>
#include <iomanip>
#include <iostream>
#include "bloom_filter.hpp"
#include <openssl/md5.h>

#include "hasher_md5.hpp"


// Преобразует 16 байт хеша в число и возвращает остаток от деления на 2^n
int md5_mod_pow2(const std::array<unsigned char, 16>& hash, int n) {
    uint64_t high = 0;
    uint64_t low = 0;

    // Первые 8 байт -> старшая часть
    for (int i = 0; i < 8; ++i) {
        high = (high << 8) | hash[i];
    }

    // Следующие 8 байт -> младшая часть
    for (int i = 8; i < 16; ++i) {
        low = (low << 8) | hash[i];
    }

    // Сложим обе части XOR-ом (как способ учесть всю энтропию)
    uint64_t combined = high ^ low;

    uint64_t mask = (1ULL << n) - 1;
    return static_cast<int>(combined & mask);
}

int main() {
    std::string input = "hello world";

    bloom_filter<hasher_md5> bf;

    return 0;
}
