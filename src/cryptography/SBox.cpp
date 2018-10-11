#include"SBox.h"
#include"../base/format.h"
#include<fstream>
#include<bitset>

using namespace std;

void SBox::Load(const string &path)
{
    ifstream loadFile;

    loadFile.open(path);

    if(!loadFile.is_open())
    {
        throw string("SBoxFile can not open");
    }

    table.clear();
    table_inverse.clear();

    char u, v;
    int u_int, v_int;
    for(int j = 0; j < tableLength; j++)
    {
        if(!(loadFile >> u >> v))
        {
            throw string("SBoxFile format is not correct");
        }

        u_int = bairuo::ToDecimal(u);
        v_int = bairuo::ToDecimal(v);
        table.insert(pair<unsigned int, unsigned int>(u_int, v_int));
        table_inverse.insert(pair<unsigned int, unsigned int>(v_int, u_int));
    }

    loadFile.close();
}

string SBox::OutPut(const string &input)   // binary string input
{
    bitset<MAXSBOXLENGTH> sub(input);
    bitset<MAXSBOXLENGTH> ans;

    unsigned int value = sub.to_ulong();

    ans = table[value];

    return bairuo::LowBinaryString<MAXSBOXLENGTH>(ans, input.length());
}

string SBox::InverseOutput(const string &input)
{
    bitset<MAXSBOXLENGTH> sub(input);
    bitset<MAXSBOXLENGTH> ans;

    unsigned int value = sub.to_ulong();

    ans = table_inverse[value];

    return bairuo::LowBinaryString<MAXSBOXLENGTH>(ans, input.length());
}
