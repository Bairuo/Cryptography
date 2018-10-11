 // SBox_H_
 // Author: Zhengru Jiang jzr98@qq.com

#ifndef SBox_H_
#define SBox_H_

#include"Box.h"
#include<map>

#define MAXSBOXLENGTH 32

class SBox : public Box
{
public:
    int tableLength;

    void Load(const std::string &path);
    std::string OutPut(const std::string &input);
    std::string InverseOutput(const std::string &input);

private:
    std::map<unsigned int, unsigned int> table;
    std::map<unsigned int, unsigned int> table_inverse;
};


#endif // SBox_H_
