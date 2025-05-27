#pragma once
#include <string>
#include <array>
#include <stdexcept>
#include <openssl/md5.h>
#include <openssl/bn.h>

template<typename hasher_type>
class hasher_adapter final
{
    unsigned long long get_hash_code(
        const std::array<
            unsigned char,
            hasher_type::hash_size>& hash,
        unsigned long long const n)
    {
        /*if (hasher_type::hash_size==0 || hasher_type::hash_size % 2 != 0)
        {
            throw std::invalid_argument("Hash size must be a multiple of 2 and more than zero");
        }*/


        BIGNUM* bn = BN_new();
        BN_bin2bn(hash.data(), hash.size(), bn);

        // Модуль
        BIGNUM* mod = BN_new();
        BN_set_word(mod, 1024); // остаток по модулю 1024

        BIGNUM* result = BN_new();
        BN_CTX* ctx = BN_CTX_new();

        BN_mod(result, bn, mod, ctx);

        BN_free(bn);
        BN_free(mod);
        BN_free(result);
        BN_CTX_free(ctx);

        return BN_get_word(result);
    }
};