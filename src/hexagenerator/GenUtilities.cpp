#include"GenUtilities.h"
#include<iostream>
#include<fstream>
#include<regex>
#include<stdlib.h>
#include<map>
#include"../base/format.h"
#include"../base/StringHandle.h"

using namespace std;

void GenerateHexa(string command)
{
    regex length_regex("\\s+?-l=[0-9]+");
    regex num_regex("\\s+?-n=[0-9]+");
    regex path_regex("-p\".*\"");
    regex show_regex("\\s+?--show=false");

    bool show = true;
    int length = -1;
    int num = 1;
    string path = "output.txt";

    smatch reg_smatch;
    sregex_iterator reg_it;
    sregex_iterator reg_end;

    reg_it = sregex_iterator(command.begin(), command.end(), show_regex);
    for(; reg_it != reg_end; ++reg_it)
    {
        int p = reg_it->position();
        if(bairuo::CalcCharBefore(command, p, '\"') % 2 == 0)
        {
            show = false;
            break;
        }
    }

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
                length += (8 - length % 8);
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

    reg_it = sregex_iterator(command.begin(), command.end(), num_regex);
    for(; reg_it != reg_end; ++reg_it)
    {
        int p = reg_it->position();
        if(bairuo::CalcCharBefore(command, p, '\"') % 2 == 0)
        {
            num = bairuo::ToInt(reg_it->str().substr(4, reg_it->str().length() - 3));

            if(num <= 0)
            {
                cout << "Num must be greater than zero\n" << endl;
                return;
            }
            break;
        }
    }

    if(regex_search(command, reg_smatch, path_regex))
    {
        path = reg_smatch.str().substr(3, reg_smatch.str().length() - 4);
    }

    map<string, bool> genMap;
    string genString;
    genString.resize(length / 4);
    int randValue;

    ofstream outputFile;
    outputFile.open(path);

    if(!outputFile.is_open())
    {
        cout << "Output file can not open" << endl;
    }

    for(int k = 0; k < num; k++)
    {
        for(int i = 0; i < length / 4; i++)
        {
            randValue = rand() % 16;
            if(randValue >= 0 && randValue < 10)
            {
                genString[i] = randValue + '0';
            }
            else
            {
                genString[i] = randValue - 10 + 'A';
            }
        }

        map<string, bool>::iterator iter = genMap.find(genString);
        if(iter != genMap.end())
        {
            k--;
            continue;
        }

        genMap.insert(pair<string, bool>(genString, true));
        outputFile << genString << endl;
        if(show)
        {
            cout << genString << endl;
        }
    }

    outputFile.close();
    cout << endl;
}
