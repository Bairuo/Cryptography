 // RSA_H_
 // Author: Zhengru Jiang jzr98@qq.com

#ifndef RSA_H_
#define RSA_H_

#include<string>

std::string RSAEncrypt(const std::string &plaintext);
std::string RSADecrypt(const std::string &ciphertext);


#endif // RSA_H_
