#include"PBox.h"
#include<fstream>

using namespace std;

void PBox::Load(const string &path)
{
    ifstream loadFile;

    loadFile.open(path);

    if(!loadFile.is_open())
    {
        throw string("PBoxFile can not open");
    }

    table.resize(tableLength);
    table_inverse.resize(tableLength);

    for(unsigned int i = 0; i < tableLength; i++)
    {
        if(!(loadFile >> table[i]))
        {
            throw string("PBoxFile format is not correct");
        }
    }

    loadFile.close();

    for(unsigned int i = 0; i < tableLength; i++)
    {
        table_inverse[table[i]] = i;
    }
}
