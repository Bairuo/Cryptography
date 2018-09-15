#include "format.h"

namespace bairuo
{
    std::string ToString(int value)
    {
        std::ostringstream os;
        os << value;
        return os.str();
    }

    int ToInt(std::string str)
    {
        return atoi(str.c_str());
    }

    int ToInt(char ch)
    {
        return ch - '0';
    }

    int ToDecimal(char c)
    {
        if(c >= '0' && c <= '9')
        {
            return c - '0';
        }
        else if(c >= 'a' && c <= 'z')
        {
            return c - 'a' + 10;
        }
        else if(c >= 'A' && c <= 'Z')
        {
            return c - 'A' + 10;
        }
        else
        {
            throw std::string("ToDecimal Error: Incorrect character format");
        }
    }

    std::string ToUpper(const std::string &lowerString)
    {
        std::string ans;
        ans.resize(lowerString.length());

        for(unsigned int i = 0; i < lowerString.length(); i++)
        {
            if(lowerString[i] > 'a' && lowerString[i] < 'z')
            {
                ans[i] = lowerString[i] + 'A' - 'a';
            }
            else
            {
                ans[i] = lowerString[i];
            }
        }

        return ans;
    }

    std::string HexaToBinaryString(const std::string &hexaString)
    {
        std::bitset<4> temp;
        std::string ans;

        for(unsigned int i = 0; i < hexaString.length(); i++)
        {
            temp = ToDecimal(hexaString[i]);
            ans += temp.to_string();
        }

        return ans;
    }

    std::string BinaryToHexaString(const std::string &binaryString)
    {
        std::string ans;
        int temp;

        if(binaryString.length() % 4 != 0)
        {
            throw std::string("ToHexa Error: Incorrect string format");
        }

        ans.resize(binaryString.length() / 4);

        for(unsigned int i = 0; i < binaryString.length(); i += 4)
        {
            temp = ToInt(binaryString[i]) * 8 + ToInt(binaryString[i + 1]) * 4
                    + ToInt(binaryString[i + 2]) * 2 + ToInt(binaryString[i + 3]) * 1;

            if(temp < 10)
            {
                ans[i / 4] = temp + '0';
            }
            else
            {
                ans[i / 4] = temp - 10 + 'A';
            }
        }
        return ans;
    }

    std::string AnyToBinaryString(const std::string &str, unsigned int byte)
    {
        std::bitset<8> temp;
        std::string ans;

        unsigned int i;
        for(i = 0; i < str.length(); i++)
        {
            temp = str[i];
            ans += temp.to_string();
        }

        for(; i < byte; i++)
        {
            ans += "00000000";
        }
        return ans;
    }

    std::string AnyToBinaryString(const std::string &str)
    {
        return AnyToBinaryString(str, str.length());
    }
}

