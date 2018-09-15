#include"EncUtilities.h"
#include"../base/format.h"
#include"../base/StringHandle.h"
#include"spn.h"
#include"rsa.h"
#include<iostream>
#include<fstream>
#include<regex>

using namespace std;

void Encrypt(string command)
{
    // regular string
    regex plaintext_regex("-m\".*?\"");
    regex spnDic_regex("-spn-d\".*?\"");
    regex inputFile_regex("-i\".*?\"");
    regex outputFile_regex("-o\".*?\"");

    // regular parameter
    regex num_regex("\\s-n=[0-9]+");
    regex algorithm_regex("\\s-[a-zA-Z]*?($|(?=\\s))");

    // regular bool
    regex detail_regex("\\s+?--detail");
    regex show_regex("\\s+?--show=false");


    string spnDic = SPN::DefaultSpnPath;
    string inputFile;
    string outputFile;
    string plaintext;
    string algorithm;
    string ciphertext;
    int num = 1;

    ifstream inputFile_stream;
    ofstream outputFile_stream;

    smatch reg_smatch;
    sregex_iterator reg_it;
    sregex_iterator reg_end;

    bool detail = false;
    bool show = true;

    reg_it = sregex_iterator(command.begin(), command.end(), detail_regex);
    for(; reg_it != reg_end; ++reg_it)
    {
        int p = reg_it->position();
        if(bairuo::CalcCharBefore(command, p, '\"') % 2 == 0)
        {
            detail = true;
            break;
        }
    }

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

    if(regex_search(command, reg_smatch, spnDic_regex))
    {
        algorithm = "spn";
#if defined(_WIN32)
        spnDic = reg_smatch.str().substr(7, reg_smatch.str().length() - 8) + "\\";
#else
        spnDic = reg_smatch.str().substr(7, reg_smatch.str().length() - 8) + "/";
#endif
    }

    if(regex_search(command, reg_smatch, outputFile_regex))
    {
        outputFile = reg_smatch.str().substr(3, reg_smatch.str().length() - 4);

        outputFile_stream.open(outputFile);
    }

    if(regex_search(command, reg_smatch, inputFile_regex))
    {
        inputFile = reg_smatch.str().substr(3, reg_smatch.str().length() - 4);

        inputFile_stream.open(inputFile);

        if(!inputFile_stream.is_open())
        {
            cout << "InputFile can not open\n" << endl;
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
    }
    else
    {
        if(regex_search(command, reg_smatch, plaintext_regex))
        {
            plaintext = reg_smatch.str().substr(3, reg_smatch.str().length() - 4);
        }
        else
        {
            cout << "No plaintext, use -m\"plaintext...\"" << endl;
            cout << "See help for more help\n" << endl;
            return;
        }
    }


    reg_it = sregex_iterator(command.begin(), command.end(), algorithm_regex);
    for(; reg_it != reg_end; ++reg_it)
    {
        int p = reg_it->position();
        if(bairuo::CalcCharBefore(command, p, '\"') % 2 == 0)
        {
            algorithm = reg_it->str().substr(2, reg_it->str().length() - 1);
            break;
        }
    }
    if(algorithm == "")
    {
        cout << "No algorithm, use -spn, -rsa, etc" << endl;
        cout << "See help for more help\n" << endl;
        return;
    }


    if(algorithm == "spn")
    {
        /****** SPN Load ******/
        try
        {
            SPN::Load(spnDic);
        }
        catch(string message)
        {
            cout << message << "\n" << endl;
            return;
        }

        for(int i = 0; i < num; i++)
        {
            /****** Encrypt ******/
            if(inputFile_stream.is_open())
            {
                if(!(inputFile_stream >> plaintext))
                {
                    cout << "Can not read plaintext" << endl;
                    cout << "Please check if there are enough plaintexts\n" << endl;
                    return;
                }
            }

            if(show)
            {
                cout << "Hexa of plaintext: " << bairuo::ToUpper(plaintext) << endl;

                try
                {
                    cout << "Binary of plaintext: " << bairuo::HexaToBinaryString(plaintext) << endl;
                }
                catch(string message)
                {
                    cout << message << endl;
                }
            }

            try
            {
                ciphertext = SPN::Encrypt(plaintext, detail);
            }
            catch(string message)
            {
                cout << message << "\n" << endl;
                return;
            }

            /****** Result Show ******/
            string hexaString = bairuo::BinaryToHexaString(ciphertext);

            if(show)
            {
                cout << "Binary of ciphertext: " << ciphertext << endl;
                cout << "Hexa of ciphertext: " << hexaString << "\n" << endl;
            }

            if(outputFile_stream.is_open())
            {
                outputFile_stream << hexaString << endl;
            }
        }

        inputFile_stream.close();
        outputFile_stream.close();

        return;
    }
    else if(algorithm == "rsa")
    {
        ciphertext = RSAEncrypt(plaintext);
    }
    else
    {
        cout << "Unknown encryption algorithm: " << algorithm << "\n" << endl;
        return;
    }
}
