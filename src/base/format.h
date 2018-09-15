// format.hpp
// Author: IS1603 jzr (Bairuo)

#ifndef Format_H_
#define Format_H_

#include <string>
#include <sstream>
#include <iostream>
#include <bitset>
#include <stdlib.h>

namespace bairuo
{
    std::string ToString(int value);
    int ToInt(std::string str);
    int ToInt(char ch);
    int ToDecimal(char c);
    std::string ToUpper(const std::string &lowerString);
    std::string HexaToBinaryString(const std::string &hexaString);
    std::string BinaryToHexaString(const std::string &binaryString);
    std::string AnyToBinaryString(const std::string &str, unsigned int byte);
    std::string AnyToBinaryString(const std::string &str);

    template<long long unsigned int T>
    std::string LowBinaryString(const std::bitset<T> &bits, unsigned int lowNum)
    {
        std::string bitString = bits.to_string();

        if(bitString.length() < lowNum)
        {
            throw std::string("LowBinaryString Error: Lack of length");
        }

        return bitString.substr(bitString.length() - lowNum, lowNum);
    }

    template<long long unsigned int T1, long long unsigned int T2>
    std::bitset<T1> SubBit(const std::bitset<T2> &bits, int start, int num)
    {
        std::string bitString = bits.to_string();

        return std::bitset<T1>(bitString.substr(start, num));
    }
}

#endif // Format_H_
