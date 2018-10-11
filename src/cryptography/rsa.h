 // RSA_H_
 // Author: Zhengru Jiang jzr98@qq.com

#ifndef RSA_H_
#define RSA_H_

#include<string>
#include <openssl/bn.h>

#include"../base/bignum.h"
#include"../base/StringHandle.h"
#include"../base/format.h"

class RSA
{
public:
    static bool init;
    static BIGNUM *n;
    static BIGNUM *b;
    static BIGNUM *p;
    static BIGNUM *q;
    static BIGNUM *a;

    static unsigned int rsaLength;

    static const std::string DefaultRsaPath;

    static const std::string ConfigFileName;
    static const std::string PublicKeyFileName;
    static const std::string PrivateKeyFileName;

    static void Load(const std::string &RsaDirectory);
    static std::string Encrypt(const std::string &plaintext, bool show = false);
    static std::string Decrypt(const std::string &ciphertext, bool show = false);
};


#endif // RSA_H_
