#include"EncUtilities.h"
#include"../base/format.h"
#include"../cryptography/spn.h"
#include<iostream>
#include<fstream>
#include<regex>
#include<bitset>

using namespace std;

void Encrypt(string command)
{
    // regular string
    regex spnDic_regex("-spn-d\".*?\"");
    regex inputFile_regex("-i\".*?\"");
    regex outputFile_regex("-o\".*?\"");

    string spnDic = SPN::DefaultSpnPath;
    string inputPath;
    string outputPath;

    smatch reg_smatch;
    sregex_iterator reg_it;
    sregex_iterator reg_end;

    if(regex_search(command, reg_smatch, spnDic_regex))
    {
#if defined(_WIN32)
        spnDic = reg_smatch.str().substr(7, reg_smatch.str().length() - 8) + "\\";
#else
        spnDic = reg_smatch.str().substr(7, reg_smatch.str().length() - 8) + "/";
#endif
    }

    if(regex_search(command, reg_smatch, inputFile_regex))
    {
        inputPath = reg_smatch.str().substr(3, reg_smatch.str().length() - 4);
    }
    else
    {
        cout << "No inputfile path, use -i\"...\"" << endl;
        cout << "See help for more help\n" << endl;
        return;
    }

    if(regex_search(command, reg_smatch, outputFile_regex))
    {
        outputPath = reg_smatch.str().substr(3, reg_smatch.str().length() - 4);
    }
    else
    {
        cout << "No outputfile path, use -i\"...\"" << endl;
        cout << "See help for more help\n" << endl;
        return;
    }

    Encrypt(spnDic, inputPath, outputPath);
}

void Encrypt(const string &spnDic, const string &inputPath, const string &outputPath)
{
    int blockLength;
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

    blockLength = SPN::blockLength;

    /****** File Open ******/
    ifstream inputFile;
    ofstream outputFile;
    inputFile.open(inputPath);
    if(!inputFile.is_open())
    {
        cout << "Can not open input file\n" << endl;
        return;
    }

    outputFile.open(outputPath, ios::binary);
    if(!outputFile.is_open())
    {
        cout << "Can not open output file\n" << endl;
        return;
    }

    /****** Encryption ******/
    bitset<MAXBLOCKLENGTH> x, y;
    string x_string;
    string y_string;
    string blockString;
    char ch[MAXBLOCKLENGTH];
    int blockByte = blockLength / 8;
    blockString.resize(blockByte);

    int totalByte;

    inputFile.seekg(0, ios::end);
    totalByte = inputFile.tellg();
    inputFile.seekg(0, ios::beg);

    float progress = 0;
    float lastProgress = -1;
    float progressDelt = 1.0f * blockLength / 8 / totalByte * 100;
    int byte = 0;

    inputFile.read(ch, blockByte * sizeof(char));
    while(inputFile.gcount() > 0)
    {
        ch[inputFile.gcount()] = '\0';
        blockString = ch;
        byte += inputFile.gcount();

        x = bitset<MAXBLOCKLENGTH>(bairuo::AnyToBinaryString(blockString, blockByte));

        if(y_string != "")
        {
            x = x ^ y;
        }

        x_string = bairuo::BinaryToHexaString(x.to_string().substr(MAXBLOCKLENGTH - 1 - blockLength, blockLength));
        y_string = SPN::Encrypt(x_string);
        y = bitset<MAXBLOCKLENGTH>(y_string);
        WriteHexaStringToFile(bairuo::BinaryToHexaString(y_string), outputFile);

        progress += progressDelt;
        if(progress - lastProgress > 1.5f)
        {
            lastProgress = progress;
            cout << "progress: " << progress << "%" << "    byte: " << byte << endl;
        }
        inputFile.read(ch, blockByte * sizeof(char));
    }

    cout << endl;

    inputFile.close();
    outputFile.close();
}

void WriteHexaStringToFile(const string &str, ofstream &file)
{
    string writeStr;
    char byte;

    writeStr.resize(str.length() / 2);

    for(unsigned int i = 0; i < str.length(); i += 2)
    {
        byte = bairuo::ToDecimal(str[i]) * 16 + bairuo::ToDecimal(str[i + 1]);
        writeStr[i / 2] = byte;
    }

    file.write(writeStr.c_str(), writeStr.length());
}
