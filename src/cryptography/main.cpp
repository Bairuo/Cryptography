#include<iostream>
#include<fstream>
#include<sstream>
#include"EncUtilities.h"
#include"DecUtilities.h"
#include"AnaUtilities.h"
#include"CraUtilities.h"

using namespace std;


#if defined(_WIN32)
static const string helpFilePath = "..\\..\\doc\\help.txt";
#else
static const string helpFilePath = "../../doc/help.txt";
#endif


int main()
{
    string command;

    cout << "Cryptography Tool" << endl;
    cout << "Copyright (c) 2018 Zhengru Jiang jzr98@qq.com" << endl;
    cout << "\tEnc\t\t---Encryption function" << endl;
    cout << "\tDec\t\t---Decryption function" << endl;
    cout << "\tAna\t\t---Analysis function" << endl;
    cout << "\tCra\t\t---Crack function" << endl;
    cout << "\tHelp\t\t---Parameter list" << endl;
    cout << "\tQuit\t\t---Close the application" << endl;
    cout << endl;

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
        else if(command.substr(0, 3) == "Enc" || command.substr(0, 3) == "enc")
        {
            Encrypt(command);
        }
        else if(command.substr(0, 3) == "Dec" || command.substr(0, 3) == "dec")
        {
            Decrypt(command);
        }
        else if(command.substr(0, 3) == "Ana" || command.substr(0, 3) == "ana")
        {
            Analysis(command);
        }
        else
        {
            cout << command + " is not a server command. See help for help\n" << endl;
        }
    }

    return 0;
}
