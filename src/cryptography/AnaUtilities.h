 // AnaUtilities_H_
 // Author: Zhengru Jiang jzr98@qq.com

#ifndef AnaUtilities_H_
#define AnaUtilities_H_

#include<string>

void Analysis(const std::string &command);
void SPNLinAnalysis(const std::string &spnPath, const std::string &plainPath, const std::string &cipherPath, int num);
void SPNDifAnalysis(const std::string &spnPath, const std::string &plainPath, const std::string &cipherPath, int num);

#endif // AnaUtilities_H_
