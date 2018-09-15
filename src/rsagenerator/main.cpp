#include<iostream>
#include<fstream>
#include<sstream>
#include<stdlib.h>
#include<time.h>
#include"GenUtilities.h"

using namespace std;

#if defined(_WIN32)
static const string helpFilePath = "..\\..\\doc\\rsagen-help.txt";
#else
static const string helpFilePath = "../../doc/rsagen-help.txt";
#endif


int main()
{
    string command;

    cout << "RSA Generator Tool" << endl;
    cout << "Copyright (c) 2018 Zhengru Jiang jzr98@qq.com" << endl;
    cout << "\tGen\t\t---Generator function" << endl;
    cout << "\tHelp\t\t---Parameter help" << endl;
    cout << "\tQuit\t\t---Close the application" << endl;
    cout << endl;

    srand(time(NULL));

    while(true)
    {
        cout << "> ";
        getline(cin, command);

        if(command.substr(0, 4) == "Quit" || command.substr(0, 4) == "quit")
        {
            return 0;
        }
        else if(command.substr(0, 4) == "Help" || command.substr(0, 4) == "help")
        {
            ifstream helpFile;
            stringstream helptxt;

            helpFile.open(helpFilePath);
            if(!helpFile.is_open())
            {
                cout << "Can not open help text\n" << endl;
                continue;
            }

            helptxt << helpFile.rdbuf();
            cout << helptxt.str() << endl;

            helpFile.close();
        }
        else if(command.substr(0, 3) == "Gen" || command.substr(0, 3) == "gen")
        {
            Generate(command);
        }
        else
        {
            cout << command + " is not a server command. See help for help\n" << endl;
        }
    }

    return 0;
}
