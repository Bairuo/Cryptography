#include"DecUtilities.h"
#include"../base/format.h"
#include"../base/StringHandle.h"
#include"spn.h"
#include"rsa.h"
#include<iostream>
#include<fstream>
#include<regex>

using namespace std;

void Decrypt(const string &command)
{
    // regular string
    regex ciphertext_regex("-m\".*?\"");
    regex spnDic_regex("-spn-d\".*?\"");
    regex rsaDic_regex("-rsa-d\".*?\"");
    regex inputFile_regex("-i\".*?\"");
    regex outputFile_regex("-o\".*?\"");

    // regular parameter
    regex num_regex("\\s-n=[0-9]+");
    regex algorithm_regex("\\s-[a-zA-Z]*?($|(?=\\s))");

    // regular bool
    regex detail_regex("\\s+?--detail");
    regex show_regex("\\s+?--show=false");


    string spnDic = SPN::DefaultSpnPath;
    string rsaDic = RSA::DefaultRsaPath;
    string inputFile;
    string outputFile;
    string ciphertext;
    string algorithm;
    string plaintext;
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
#if defined(_WIN32)
        spnDic = reg_smatch.str().substr(7, reg_smatch.str().length() - 8) + "\\";
#else
        spnDic = reg_smatch.str().substr(7, reg_smatch.str().length() - 8) + "/";
#endif
    }
    else if(regex_search(command, reg_smatch, rsaDic_regex))
    {
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
        if(regex_search(command, reg_smatch, ciphertext_regex))
        {
            ciphertext = reg_smatch.str().substr(3, reg_smatch.str().length() - 4);
        }
        else
        {
            cout << "No ciphertext, use -m\"ciphertext...\"" << endl;
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
                if(!(inputFile_stream >> ciphertext))
                {
                    cout << "Can not read ciphertext" << endl;
                    cout << "Please check if there are enough ciphertexts\n" << endl;
                    return;
                }
            }

            if(show)
            {
                cout << "Hexa of ciphertext: " << bairuo::ToUpper(ciphertext) << endl;

                try
                {
                    cout << "Binary of ciphertext: " << bairuo::HexaToBinaryString(ciphertext) << endl;
                }
                catch(string message)
                {
                    cout << message << endl;
                }
            }

            try
            {
                plaintext = SPN::Decrypt(ciphertext, detail);
            }
            catch(string message)
            {
                cout << message << "\n" << endl;
                return;
            }

            /****** Result Show ******/
            string hexaString = bairuo::BinaryToHexaString(plaintext);

            if(show)
            {
                cout << "Binary of plaintext: " << plaintext << endl;
                cout << "Hexa of plaintext: " << hexaString << "\n" << endl;
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
        /****** RSA Load ******/
        try
        {
            RSA::Load(rsaDic);
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
                if(!(inputFile_stream >> ciphertext))
                {
                    cout << "Can not read ciphertext" << endl;
                    cout << "Please check if there are enough ciphertexts\n" << endl;
                    return;
                }
            }

            if(show)
            {
                cout << "Hexa of ciphertext: " << bairuo::ToUpper(ciphertext) << endl;
            }

            try
            {
                plaintext = RSA::Decrypt(ciphertext, detail);
            }
            catch(string message)
            {
                cout << message << "\n" << endl;
                return;
            }

            /****** Result Show ******/

            if(show)
            {
                cout << "Hexa of plaintext: " << plaintext << "\n" << endl;
            }

            if(outputFile_stream.is_open())
            {
                outputFile_stream << plaintext << endl;
            }
        }

        inputFile_stream.close();
        outputFile_stream.close();

        return;
    }
    else
    {
        cout << "Unknown encryption algorithm: " << algorithm << "\n" << endl;
        return;
    }
}
