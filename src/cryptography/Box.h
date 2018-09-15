 // Box_H_
 // Author: Zhengru Jiang jzr98@qq.com

#ifndef Box_H_
#define Box_H_

#include<string>

class Box
{
public:
    virtual void Load(const std::string &path){}
    virtual std::string OutPut(std::string input) = 0;
};



#endif // Box_H_
