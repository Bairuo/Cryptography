#include "bignum.h"
#include "format.h"
#include <string>
#include <iostream>

namespace bairuo
{
#if defined(_WIN32)
BIGNUM* MontModExp(const BIGNUM *a, const BIGNUM *b, const BIGNUM *n)
{
    BN_CTX *ctx = BN_CTX_new();
    BIGNUM *_a = BN_new();
    BIGNUM *_b = BN_new();
    BIGNUM *zero = BN_new();
    BIGNUM *ans = BN_new();
    BN_copy(_a, a);
    BN_copy(_b, b);

#ifdef FUNCTIONTIMETEST
    clock_t startClock, endClock;
    startClock = clock();
#endif // FUNCTIONTIMETEST

    BN_zero(zero);
    BN_one(ans);
    while(BN_cmp(_b, zero) > 0)
    {
        if(BN_is_odd(_b))
        {
            BN_mod_mul(ans, ans, _a, n, ctx);
        }
        BN_rshift1(_b, _b);
        BN_mod_sqr(_a, _a, n, ctx);
    }

#ifdef FUNCTIONTIMETEST
    endClock = clock();
    int timecounter = endClock - startClock;
    std::cout << "Mont Exp: " << timecounter << "ms" << std::endl;
#endif // FUNCTIONTIMETEST

    BN_free(_a);
    BN_free(_b);
    BN_free(zero);
    BN_CTX_free(ctx);

    return ans;
}
#endif // defined

BIGNUM* ModExp(const BIGNUM *a, const BIGNUM *b, const BIGNUM *n)
{
    BN_CTX *ctx = BN_CTX_new();
    BIGNUM *z = BN_new();
    int l = BN_num_bits(b);
    std::string b_hex = BN_bn2hex(b);
    std::string b_binary = bairuo::HexaToBinaryString(b_hex);
    int start = b_binary.length() - l;

#ifdef FUNCTIONTIMETEST
    clock_t startClock, endClock;
    startClock = clock();
#endif // FUNCTIONTIMETEST

    BN_one(z);
    for(unsigned int i = start; i < b_binary.length(); i++)
    {
        BN_mod_sqr(z, z, n, ctx);
        if(b_binary[i] == '1')
        {
            BN_mod_mul(z, z, a, n, ctx);
        }
    }

    BN_CTX_free(ctx);

#ifdef FUNCTIONTIMETEST
    endClock = clock();
    int timecounter = endClock - startClock;
#if defined(_WIN32)
    std::cout << "Square-and-Multiply: " << timecounter << "ms" << std::endl;
#else
    std::cout << "Square-and-Multiply: " << timecounter << "us" << std::endl;
#endif // defined
#endif // FUNCTIONTIMETEST

    return z;
}

bool MillerRabin(const BIGNUM *n)
{
    bool IsPrime = false;
    BN_CTX *ctx = BN_CTX_new();
    BIGNUM *k = BN_new();
    BIGNUM *n1 = BN_new();
    BIGNUM *zero = BN_new();
    BIGNUM *one = BN_new();
    BIGNUM *two = BN_new();
    BIGNUM *m = BN_new();
    BIGNUM *r = BN_new();

    BN_one(one);
    BN_zero(zero);
    BN_add(two, one, one);
    BN_sub(n1, n, one);

    // get m, k
    BN_mod(r, n, two, ctx);
    if(BN_cmp(r, zero) == 0)
    {
        BN_CTX_free(ctx);
        return false;
    }

    BN_div(m, NULL, n1, two, ctx);
    BN_one(k);
    BN_mod(r, m, two, ctx);
    while(BN_cmp(r, zero) == 0)
    {
        BN_div(m, NULL, m, two, ctx);
        BN_add(k, k, one);
        BN_mod(r, m, two, ctx);
    }
    // get a, b
    BIGNUM *a = BN_new();
    BN_rand_range(a, n1);
    while(BN_cmp(a, zero) == 0)
    {
        BN_rand_range(a, n1);
    }
    BIGNUM *b = ModExp(a, m, n);
    // use openssl
//        BIGNUM *b = BN_new();
//        BN_mod_exp(b, a, m, n, ctx);

    // prime judge
    if(BN_cmp(b, one) == 0)
    {
        IsPrime = true;
    }
    BIGNUM *t = BN_new();
    BN_zero(t);
    while(BN_cmp(t, k) != 0)
    {
        BN_mod(r, b, n, ctx);
        if(BN_cmp(r, n1) == 0)
        {
            IsPrime = true;
            break;
        }
        BN_mod_sqr(b, b, n, ctx);
        BN_add(t, t, one);
    }

    BN_CTX_free(ctx);
    BN_free(k);
    BN_free(n1);
    BN_free(zero);
    BN_free(one);
    BN_free(two);
    BN_free(m);
    BN_free(r);
    BN_free(a);
    BN_free(t);
    return IsPrime;
}

bool IsPrime(const BIGNUM *num)
{
    for(int i = 0; i < 4; i++)
    {
        if(MillerRabin(num))
        {
            return true;
        }
    }
    return false;
}

// openssl
//    BIGNUM* gcd(BIGNUM *a, BIGNUM *b)
//    {
//        BN_CTX *ctx = BN_CTX_new();
//        BIGNUM *r = BN_new();
//
//        BN_gcd(r, a, b, ctx);
//
//        BN_CTX_free(ctx);
//        return r;
//    }

BIGNUM* exgcd(const BIGNUM *a, const BIGNUM *b, BIGNUM *x, BIGNUM *y)
{
    BN_CTX *ctx = BN_CTX_new();
    BIGNUM *zero = BN_new();
    BIGNUM *gcda = BN_new();
    BIGNUM *gcdb = BN_new();
    BIGNUM *t1 = BN_new();
    BIGNUM *t2 = BN_new();
    BIGNUM *r0 = BN_new();
    BIGNUM *r;

    if(BN_cmp(a, b) == 1)
    {
        BN_copy(gcda, a);
        BN_copy(gcdb, b);
    }
    else
    {
        BN_copy(gcda, b);
        BN_copy(gcdb, a);
    }

    if(BN_cmp(gcdb, zero) == 0)
    {
        BN_one(x);
        BN_zero(y);

        BN_CTX_free(ctx);
        BN_free(zero);
        BN_free(gcdb);
        BN_free(t1);
        BN_free(t2);
        BN_free(r0);
        return gcda;
    }
    BN_mod(r0, gcda, gcdb, ctx);
    r = exgcd(gcdb, r0, x, y);
    BN_copy(t1, x);
    BN_copy(x, y);
    BN_div(t2, NULL, gcda, gcdb, ctx);
    BN_mul(t2, t2, y, ctx);
    BN_sub(y, t1, t2);

    BN_CTX_free(ctx);
    BN_free(zero);
    BN_free(gcda);
    BN_free(gcdb);
    BN_free(t1);
    BN_free(t2);
    BN_free(r0);
    return r;
}

BIGNUM* gcd(const BIGNUM *a, const BIGNUM *b)
{
    BN_CTX *ctx = BN_CTX_new();
    const BIGNUM *t;
    BIGNUM *gcda = BN_new();
    BIGNUM *gcdb = BN_new();
    BIGNUM *zero = BN_new();
    BIGNUM *r = BN_new();
    BN_zero(zero);

    // a must greater than b
    if(BN_cmp(a, b) == -1)
    {
        t = a;
        a = b;
        b = t;
    }

    BN_copy(gcda, a);
    BN_copy(gcdb, b);

    while(BN_cmp(gcdb, zero) != 0)
    {
        BN_mod(r, gcda, gcdb, ctx);
        BN_copy(gcda, gcdb);
        BN_copy(gcdb, r);
    }
    BN_copy(r, gcda);

    BN_CTX_free(ctx);
    BN_free(zero);
    BN_free(gcda);
    BN_free(gcdb);
    return r;
}

BIGNUM* modInverse(const BIGNUM *a, const BIGNUM *n)
{
    BN_CTX *ctx = BN_CTX_new();
    BIGNUM *r = BN_new();
    BIGNUM *x = BN_new();
    BIGNUM *y = BN_new();

    exgcd(a, n, x, y);

    if(BN_cmp(a, n) == -1)
    {
        BN_copy(r, y);
    }
    else
    {
        BN_copy(r, x);
    }
    BN_nnmod(r, r, n, ctx);

    BN_CTX_free(ctx);
    BN_free(x);
    BN_free(y);
    return r;
}

// openssl
//    BIGNUM* modInverse(const BIGNUM *a, const BIGNUM *n)
//    {
//        BN_CTX *ctx = BN_CTX_new();
//        BIGNUM *r = BN_new();
//
//        BN_mod_inverse(r, a, n, ctx);
//
//        BN_CTX_free(ctx);
//        return r;
//    }

BIGNUM *genBigPrime(int length)
{
    BN_CTX *ctx = BN_CTX_new();
    BIGNUM *p = BN_new();
    BIGNUM *t1 = BN_new();
    BIGNUM *t2 = BN_new();
    BN_set_word(t1, 229);
    BN_set_word(t2, 197);
    BN_rand(p, length, 1, 0);

    while(!IsPrime(p))
    {
        BN_rand(p, length, 1, 0);
    }

    BN_CTX_free(ctx);
    BN_free(t1);
    BN_free(t2);
    return p;
}
}
