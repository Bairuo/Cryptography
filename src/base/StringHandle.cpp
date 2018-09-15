#include "StringHandle.h"

namespace bairuo
{
    int CalcCharBefore(const std::string &str, unsigned int pos, char ch)
    {
        int ans = 0;

        if(pos >= str.length())
        {
            throw std::string("StringHandle Error: Position over length");
        }

        for(unsigned int i = 0; i < pos; i++)
        {
            if(str[i] == ch)
            {
                ans++;
            }
        }

        return ans;
    }
}
