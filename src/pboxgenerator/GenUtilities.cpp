#include"GenUtilities.h"
#include<iostream>
#include<fstream>
#include<regex>
#include<stdlib.h>
#include<vector>
#include"../base/format.h"
#include"../base/StringHandle.h"

using namespace std;

void GenerateHexa(string command)
{
    regex length_regex("\\s+?-l=[0-9]+");
    regex path_regex("-p\".*\"");

    int length = -1;
    string path = "PBOX.txt";

    smatch reg_smatch;
    sregex_iterator reg_it;
    sregex_iterator reg_end;

    reg_it = sregex_iterator(command.begin(), command.end(), length_regex);
    for(; reg_it != reg_end; ++reg_it)
    {
        int p = reg_it->position();
        if(bairuo::CalcCharBefore(command, p, '\"') % 2 == 0)
        {
            length = bairuo::ToInt(reg_it->str().substr(4, reg_it->str().length() - 3));

            if(length <= 0)
            {
                cout << "String length must be greater than zero\n" << endl;
                return;
            }

            if(length % 8 != 0)
            {
                cout << "The number of bit must be a multiple of eight\n" << endl;
                return;
            }

            break;
        }
    }
    if(length == -1)
    {
        cout << "No specified length , use -l= " << endl;
        cout << "See help for more help\n" << endl;
        return;
    }

    if(regex_search(command, reg_smatch, path_regex))
    {
        path = reg_smatch.str().substr(3, reg_smatch.str().length() - 4);
    }

    ofstream outputFile;
    outputFile.open(path);

    if(!outputFile.is_open())
    {
        cout << "Output file can not open" << endl;
    }

    vector<int> randomValues;
    randomValues.resize(length);
    int index;

    for(int i = 0; i < length; i++)
    {
        randomValues[i] = i;
    }

    for(int i = 0; i < length; i++)
    {
        index = rand() % (length - i);

        outputFile << randomValues[index] << " ";
        cout << randomValues[index] << " ";

        vector<int>::iterator it = randomValues.begin() + index;
        randomValues.erase(it);
    }

    outputFile.close();
    cout << "\n" << endl;
}
