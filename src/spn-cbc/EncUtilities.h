 // EncUtilities_H_
 // Author: Zhengru Jiang jzr98@qq.com

#ifndef EncUtilities_H_
#define EncUtilities_H_

#include<string>
#include<fstream>

void Encrypt(std::string command);
void Encrypt(const std::string &spnDic, const std::string &inputPath, const std::string &outputPath);
void WriteHexaStringToFile(const std::string &str, std::ofstream &file);


#endif // EncUtilities_H_
