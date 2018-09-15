 // PBox_H_
 // Author: Zhengru Jiang jzr98@qq.com

#ifndef PBox_H_
#define PBox_H_

#include<vector>
#include<map>
#include<bitset>
#include<iostream>

class PBox
{
public:
    unsigned int tableLength;

    void Load(const std::string &path);

    template<long long unsigned int T>
    std::bitset<T> OutPut(const std::bitset<T> &input)
    {
        std::bitset<T> ans;

        ans.reset();

        long long unsigned int i;

        for(i = 0; i < tableLength; i++)
        {
            ans[i] = input[table[i]];
        }

        return ans;
    }

    template<long long unsigned int T>
    std::bitset<T> InverseOutput(const std::bitset<T> &input)
    {
        std::bitset<T> ans;

        ans.reset();

        long long unsigned int i;

        for(i = 0; i < tableLength; i++)
        {
            ans[i] = input[table_inverse[i]];
        }

        return ans;
    }

private:
    std::vector<int> table;
    std::vector<int> table_inverse;
};


#endif // PBox_H_
