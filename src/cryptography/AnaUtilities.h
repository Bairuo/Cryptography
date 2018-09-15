 // AnaUtilities_H_
 // Author: Zhengru Jiang jzr98@qq.com

#ifndef AnaUtilities_H_
#define AnaUtilities_H_

#include<string>

void Analysis(std::string command);
void SPNLinAnalysis(std::string spnPath, std::string plainPath, std::string cipherPath, int num);
void SPNDifAnalysis(std::string spnPath, std::string plainPath, std::string cipherPath, int num);

#endif // AnaUtilities_H_
