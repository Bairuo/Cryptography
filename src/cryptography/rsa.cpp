#include "rsa.h"
#include"../base/format.h"
#include<fstream>
#include<iostream>

#define ADMINISTRATORS
#define TIMETEST

using namespace std;

bool RSA::init = false;
BIGNUM* RSA::n = nullptr;
BIGNUM* RSA::b = nullptr;
BIGNUM* RSA::p = nullptr;
BIGNUM* RSA::q = nullptr;
BIGNUM* RSA::a = nullptr;

unsigned int RSA::rsaLength = 1024;

#if defined(_WIN32)
const string RSA::DefaultRsaPath = "..\\..\\res\\RSA\\";
#else
const string RSA::DefaultRsaPath = "../../res/RSA/";
#endif

const string RSA::ConfigFileName = "config";
const string RSA::PublicKeyFileName = "public.txt";
const string RSA::PrivateKeyFileName = "private.txt";

void RSA::Load(const string &RsaDirectory)
{
    ifstream configFile;
    ifstream publickeyFile;
    ifstream privatekeyFile;

    /****** Configuration Load ******/
    configFile.open(RsaDirectory + ConfigFileName);

    if(configFile.is_open())
    {
        if(!(configFile >> rsaLength))
        {
            throw string("Config file format error");
        }
    }

    configFile.close();

    /****** Public Key Load ******/
    publickeyFile.open(RsaDirectory + PublicKeyFileName);
    privatekeyFile.open(RsaDirectory + PrivateKeyFileName);

    if(!publickeyFile.is_open())
    {
        throw string("PublicKey File can not open");
    }

    if(!privatekeyFile.is_open())
    {
        throw string("PrivateKey File can not open");
    }

    string nstr, bstr;
    string pstr, qstr, astr;

    if(init)
    {
        BN_free(n);
        BN_free(b);
        BN_free(p);
        BN_free(q);
        BN_free(a);
    }
    n = BN_new();
    b = BN_new();
    p = BN_new();
    q = BN_new();
    a = BN_new();

    publickeyFile >> nstr >> bstr;
    privatekeyFile >> pstr >> qstr >> astr;

    BN_hex2bn(&n, nstr.c_str());
    BN_hex2bn(&b, bstr.c_str());
    BN_hex2bn(&p, pstr.c_str());
    BN_hex2bn(&q, qstr.c_str());
    BN_hex2bn(&a, astr.c_str());

    publickeyFile.close();
    privatekeyFile.close();

    init = true;
}

string RSA::Encrypt(const string &plaintext, bool show)
{
    BIGNUM *M1 = q;
    BIGNUM *M2 = p;
    BIGNUM *M1R = BN_new();
    BIGNUM *M2R = BN_new();
    BIGNUM *plain = BN_new();
    BIGNUM *cipher = BN_new();
    BIGNUM *t1 = BN_new();
    BIGNUM *t2 = BN_new();
    BN_CTX *ctx = BN_CTX_new();

    BN_hex2bn(&plain, plaintext.c_str());

#ifdef TIMETEST
    clock_t startClock, endClock;
    startClock = clock();
#endif // TIMETEST

    M1R = bairuo::modInverse(M1, p);
    M2R = bairuo::modInverse(M2, q);

#ifdef ADMINISTRATORS
    // only administrators can use p, q
    BIGNUM *a1, *a2;
    BIGNUM *one = BN_new();
    BIGNUM *bt1 = BN_new();
    BIGNUM *bt2 = BN_new();
    BIGNUM *psub1 = BN_new();
    BIGNUM *qsub1 = BN_new();

    BN_one(one);
    BN_sub(psub1, p, one);
    BN_sub(qsub1, q, one);
    BN_mod(bt1, b, psub1, ctx);
    BN_mod(bt2, b, qsub1, ctx);

    a1 = bairuo::ModExp(plain, bt1, p);
    a2 = bairuo::ModExp(plain, bt2, q);

    BN_free(one);
    BN_free(bt1);
    BN_free(bt2);
    BN_free(psub1);
    BN_free(qsub1);

    BN_mul(t1, M1, M1R, ctx);
    BN_mul(t1, t1, a1, ctx);
    BN_mul(t2, M2, M2R, ctx);
    BN_mul(t2, t2, a2, ctx);

    BN_free(a1);
    BN_free(a2);

    BN_mod_add(cipher, t1, t2, n, ctx);
#else
    // for ordinary user
    // RSA::UserEncrypt
    cipher = bairuo::ModExp(plain, b, n);
#endif // ADMINISTRATORS

#ifdef TIMETEST
    endClock = clock();
    int timecounter = endClock - startClock;
#if defined(_WIN32)
    std::cout << "RSA Encryption: " << timecounter << "ms" << std::endl;
#else
    std::cout << "RSA Encryption: " << timecounter << "us" << std::endl;
#endif // defined
#if defined(_WIN32)
    BN_free(bairuo::MontModExp(plain, b, p));
#endif // defined
#endif // TIMETEST

    string ans = BN_bn2hex(cipher);


    BN_free(M1R);
    BN_free(M2R);
    BN_free(plain);
    BN_free(cipher);
    BN_free(t1);
    BN_free(t2);
    BN_CTX_free(ctx);

    return ans;
}

string RSA::Decrypt(const string &ciphertext, bool show)
{
    BIGNUM *M1 = q;
    BIGNUM *M2 = p;
    BIGNUM *M1R = BN_new();
    BIGNUM *M2R = BN_new();
    BIGNUM *plain = BN_new();
    BIGNUM *cipher = BN_new();
    BIGNUM *t1 = BN_new();
    BIGNUM *t2 = BN_new();
    BN_CTX *ctx = BN_CTX_new();

    BN_hex2bn(&cipher, ciphertext.c_str());

#ifdef TIMETEST
    clock_t startClock, endClock;
    startClock = clock();
#endif // TIMETEST

    M1R = bairuo::modInverse(M1, p);
    M2R = bairuo::modInverse(M2, q);

#if defined(MODEXPTEST)
    plain = bairuo::ModExp(cipher, a, n);
#elif defined(MONTTEST)
#if defined(_WIN32)
    plain = bairuo::MontModExp(cipher, a, n);
#else
    plain = bairuo::ModExp(cipher, a, n);
#endif // defined
#else
    BIGNUM *a1, *a2;
    BIGNUM *one = BN_new();
    BIGNUM *at1 = BN_new();
    BIGNUM *at2 = BN_new();
    BIGNUM *psub1 = BN_new();
    BIGNUM *qsub1 = BN_new();

    BN_one(one);
    BN_sub(psub1, p, one);
    BN_sub(qsub1, q, one);
    BN_mod(at1, a, psub1, ctx);
    BN_mod(at2, a, qsub1, ctx);

//    a1 = bairuo::ModExp(cipher, at1, p);
//    a2 = bairuo::ModExp(cipher, at2, q);
    a1 = bairuo::MontModExp(cipher, at1, p);
    a2 = bairuo::MontModExp(cipher, at2, q);

    BN_free(one);
    BN_free(at1);
    BN_free(at2);
    BN_free(psub1);
    BN_free(qsub1);

    BN_mul(t1, M1, M1R, ctx);
    BN_mul(t1, t1, a1, ctx);
    BN_mul(t2, M2, M2R, ctx);
    BN_mul(t2, t2, a2, ctx);

    BN_free(a1);
    BN_free(a2);

    BN_mod_add(plain, t1, t2, n, ctx);
#endif // ndefiend

#ifdef TIMETEST
    endClock = clock();
    int timecounter = endClock - startClock;
#if defined(_WIN32)
    std::cout << "RSA Decryption: " << timecounter << "ms" << std::endl;
#else
    std::cout << "RSA Decryption: " << timecounter << "us" << std::endl;
#endif // defined
#endif // TIMETEST

    string ans = BN_bn2hex(plain);

    BN_free(M1R);
    BN_free(M2R);
    BN_free(plain);
    BN_free(cipher);
    BN_free(t1);
    BN_free(t2);
    BN_CTX_free(ctx);
    return ans;
}
