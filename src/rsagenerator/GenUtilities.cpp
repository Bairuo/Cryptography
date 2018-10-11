#include"GenUtilities.h"
#include<iostream>
#include<fstream>
#include<regex>
#include<stdlib.h>
#include <openssl/bn.h>
#include"../base/bignum.h"
#include"../base/StringHandle.h"
#include"../base/format.h"

using namespace std;

void Generate(string command)
{
    regex length_regex("\\s+?-l=[0-9]+");
    regex rsaDic_regex("-d\".*\"");

    int length = -1;
    string rsaDic = "RSA\\";

    smatch reg_smatch;
    sregex_iterator reg_it;
    sregex_iterator reg_end;

    reg_it = sregex_iterator(command.begin(), command.end(), length_regex);
    for(; reg_it != reg_end; ++reg_it)
    {
        int p = reg_it->position();
        if(bairuo::CalcCharBefore(command, p, '\"') % 2 == 0)
        {
            length = bairuo::ToInt(reg_it->str().substr(4, reg_it->str().length() - 3));

            if(length <= 0)
            {
                cout << "String length must be greater than zero\n" << endl;
                return;
            }

            if(length % 8 != 0)
            {
                cout << "The number of bit must be a multiple of eight\n" << endl;
                return;
            }

            break;
        }
    }
    if(length == -1)
    {
        cout << "No specified length , use -l= " << endl;
        cout << "See help for more help\n" << endl;
        return;
    }

    if(regex_search(command, reg_smatch, rsaDic_regex))
    {
#if defined(_WIN32)
        rsaDic = reg_smatch.str().substr(3, reg_smatch.str().length() - 4) + "\\";
#else
        rsaDic = reg_smatch.str().substr(3, reg_smatch.str().length() - 4) + "/";
#endif
    }

    ofstream config;
    ofstream privateKey;
    ofstream publicKey;
    config.open(rsaDic + "config");
    privateKey.open(rsaDic + "private.txt");
    publicKey.open(rsaDic + "public.txt");

    if(!privateKey.is_open() || !publicKey.is_open() || !config.is_open())
    {
        cout << "Unknown error: File can not open" << "\n" << endl;
        return;
    }

    config << length;
    config.close();

    /****** parameter generator ******/
    BIGNUM *p = bairuo::genBigPrime(length);

    BIGNUM *q = bairuo::genBigPrime(length);
    while(BN_cmp(q, p) == 0)
    {
        BN_free(q);
        q = bairuo::genBigPrime(length);
    }

    BIGNUM *n = BN_new();
    BN_CTX *ctx = BN_CTX_new();

    BN_mul(n, p, q, ctx);

    BIGNUM *fn = BN_new();
    BIGNUM *q1 = BN_new();
    BIGNUM *p1 = BN_new();
    BIGNUM *one = BN_new();
    BN_one(one);
    BN_sub(p1, p, one);
    BN_sub(q1, q, one);

    BN_mul(fn, p1, q1, ctx);

    BIGNUM *b = BN_new();
    BN_rand_range(b, fn);
    while(BN_cmp(b, one) == 0 || BN_cmp(bairuo::gcd(b, fn), one) != 0)
    {
        BN_rand_range(b, fn);
    }

    BIGNUM *a = bairuo::modInverse(b, fn);

    cout << "public key n,b: " << endl;
    cout << BN_bn2hex(n) << endl;
    cout << BN_bn2hex(b) << "\n" << endl;
    publicKey << BN_bn2hex(n) << endl;
    publicKey << BN_bn2hex(b) << endl;

    cout << "private key p,q,a: " << endl;
    cout << BN_bn2hex(p) << endl;
    cout << BN_bn2hex(q) << endl;
    cout << BN_bn2hex(a) <<  "\n" << endl;
    privateKey << BN_bn2hex(p) << endl;
    privateKey << BN_bn2hex(q) << endl;
    privateKey << BN_bn2hex(a) << endl;

    BN_CTX_free(ctx);
    BN_free(n);
    BN_free(b);
    BN_free(fn);
    BN_free(q1);
    BN_free(p1);
    BN_free(one);

    privateKey.close();
    publicKey.close();
}
