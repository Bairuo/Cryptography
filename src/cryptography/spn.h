 // SPN_H_
 // Author: Zhengru Jiang jzr98@qq.com

#ifndef SPN_H_
#define SPN_H_

#include<string>
#include<vector>
#include<map>
#include<bitset>
#include"SBox.h"
#include"PBox.h"

#define MAXBLOCKLENGTH 128

class SPN
{
public:
    static bool init;

    static int wheelNum;
    static unsigned int sboxLength;
    static unsigned int blockLength;

    static std::vector<std::bitset<MAXBLOCKLENGTH> >keys;
    static std::vector<SBox>sboxs;
    static PBox pbox;

    static const std::string DefaultSpnPath;

    static const std::string ConfigFileName;
    static const std::string SpnKeyFileName;
    static const std::string SpnPBoxFileName;
    static const std::string SpnSBoxName;

    static void Load(const std::string &SpnDirectory);
    static std::string Encrypt(const std::string &plaintext, bool show = false);
    static std::string Decrypt(const std::string &ciphertext, bool show = false);
};


#endif // SPN_H_
