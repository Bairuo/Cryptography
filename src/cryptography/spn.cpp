#include"spn.h"
#include"../base/format.h"
#include<fstream>
#include<iostream>

using namespace std;

bool SPN::init = false;

int SPN::wheelNum = 4;
unsigned int SPN::sboxLength = 4;   // bit
unsigned int SPN::blockLength = 16;      // bit

vector<bitset<MAXBLOCKLENGTH> > SPN::keys;
PBox SPN::pbox;
std::vector<SBox> SPN::sboxs;

#if defined(_WIN32)
const string SPN::DefaultSpnPath = "..\\..\\res\\SPN-16\\";
#else
const string SPN::DefaultSpnPath = "../../res/SPN-16/";
#endif

const string SPN::ConfigFileName = "config";
const string SPN::SpnKeyFileName = "SPNK.txt";
const string SPN::SpnPBoxFileName = "PBOX.txt";
const string SPN::SpnSBoxName = "SBOX";


void SPN::Load(const string &SpnDirectory)
{
    ifstream configFile;
    ifstream keyFile;

    /****** Configuration Load ******/
    configFile.open(SpnDirectory + ConfigFileName);

    if(configFile.is_open())
    {
        if(!(configFile >> blockLength >> wheelNum >> sboxLength))
        {
            throw string("Config file format error");
        }
    }

    /****** Key Load ******/
    keyFile.open(SpnDirectory + SpnKeyFileName);

    if(!keyFile.is_open())
    {
        throw string("KeyFile can not open");
    }

    keys.resize(wheelNum + 1);
    string hexaString;

    for(int i = 0; i <= wheelNum; i++)      // key number is wheelNum + 1
    {
        if(!getline(keyFile, hexaString) || hexaString.length() * 4 != blockLength)
        {
            throw string("KeyFile format is not correct");
        }

        keys[i] = bitset<MAXBLOCKLENGTH>(bairuo::HexaToBinaryString(hexaString));

        //cout << bairuo::LowBinaryString(keys[i], 16) << endl;
    }

    keyFile.close();

    /****** SBox Load ******/
    sboxs.resize(wheelNum);

    for(int i = 0; i < wheelNum; i++)
    {
        sboxs[i].tableLength = 1 << sboxLength;

        try
        {
            sboxs[i].Load(SpnDirectory + SpnSBoxName + bairuo::ToString(i + 1) + ".txt");
        }
        catch(string message)
        {
            throw message;
        }
    }

    /****** PBox Load ******/
    pbox.tableLength = blockLength;

    try
    {
        pbox.Load(SpnDirectory + SpnPBoxFileName);
    }
    catch(string message)
    {
        throw message;
    }

    init = true;
}

string SPN::Encrypt(const string &plaintext, bool show)
{
    if(!init)
    {
        throw string("Spn not init");
    }

    if(plaintext.length() * 4 != blockLength)
    {
        throw string("Plaintext length error");
    }

    // Encryption
    bitset<MAXBLOCKLENGTH> w(bairuo::HexaToBinaryString(plaintext));
    bitset<MAXBLOCKLENGTH> u;
    string u_string;
    string v;

    if(show)
    {
        cout << "w: " << bairuo::LowBinaryString<MAXBLOCKLENGTH>(w, blockLength) << endl;
    }

    for(int i = 0; i < wheelNum - 1; i++)
    {
        if(show)
        {
            cout << "k: " << bairuo::LowBinaryString<MAXBLOCKLENGTH>(keys[i], blockLength) << endl;
        }

        u = w ^ keys[i];
        v = "";

        if(show)
        {
            cout << "u: " << bairuo::LowBinaryString<MAXBLOCKLENGTH>(u, blockLength) << endl;
        }

        u_string = bairuo::LowBinaryString<MAXBLOCKLENGTH>(u, blockLength);
        for(unsigned int j = 0; j < blockLength; j+= sboxLength)
        {
            v += sboxs[i].OutPut(u_string.substr(j, sboxLength));      // binary string
        }

        if(show)
        {
            cout << "v: " << v << endl;
        }

        w = bitset<MAXBLOCKLENGTH>(v);
        w = pbox.OutPut<MAXBLOCKLENGTH>(w);

        if(show)
        {
            cout << "w: " << bairuo::LowBinaryString<MAXBLOCKLENGTH>(w, blockLength) << endl;
        }
    }

    u = w ^ keys[wheelNum - 1];

    if(show)
    {
        cout << "u: " << bairuo::LowBinaryString<MAXBLOCKLENGTH>(u, blockLength) << endl;
    }

    v = "";

    u_string = bairuo::LowBinaryString<MAXBLOCKLENGTH>(u, blockLength);
    for(unsigned int j = 0; j < blockLength; j+= sboxLength)
    {
        v += sboxs[wheelNum - 1].OutPut(u_string.substr(j, sboxLength));
    }

    if(show)
    {
        cout << "v: " << v << endl;
    }

    w = bitset<MAXBLOCKLENGTH>(v);

    if(show)
    {
        cout << "w: " << bairuo::LowBinaryString<MAXBLOCKLENGTH>(w, blockLength) << endl;
        cout << "k: " << bairuo::LowBinaryString<MAXBLOCKLENGTH>(keys[wheelNum], blockLength) << "\n" << endl;
    }


    bitset<MAXBLOCKLENGTH> y = w ^ keys[wheelNum];

    return bairuo::LowBinaryString<MAXBLOCKLENGTH>(y, blockLength);
}


string SPN::Decrypt(const string &ciphertext, bool show)
{
    if(!init)
    {
        throw string("Spn not init");
    }

    if(ciphertext.length() * 4 != blockLength)
    {
        throw string("Ciphertext length error");
    }

    // Encryption
    bitset<MAXBLOCKLENGTH> w(bairuo::HexaToBinaryString(ciphertext));
    bitset<MAXBLOCKLENGTH> u;
    bitset<MAXBLOCKLENGTH> v;
    string u_string;
    string v_string;

    u = w ^ keys[wheelNum];

    v_string = "";
    u_string = bairuo::LowBinaryString<MAXBLOCKLENGTH>(u, blockLength);
    for(unsigned int j = 0; j < blockLength; j+= sboxLength)
    {
        v_string += sboxs[0].InverseOutput(u_string.substr(j, sboxLength));
    }

    v = bitset<MAXBLOCKLENGTH>(v_string);
    u = v ^ keys[wheelNum - 1];

    for(int i = wheelNum - 2; i >= 0; i--)
    {
        w = pbox.InverseOutput<MAXBLOCKLENGTH>(u);

        v_string = "";

        u_string = bairuo::LowBinaryString<MAXBLOCKLENGTH>(w, blockLength);
        for(unsigned int j = 0; j < blockLength; j+= sboxLength)
        {
            v_string += sboxs[i].InverseOutput(u_string.substr(j, sboxLength));      // binary string
        }

        u = bitset<MAXBLOCKLENGTH>(v_string);
        u = u ^ keys[i];
    }

    return bairuo::LowBinaryString<MAXBLOCKLENGTH>(u, blockLength);
}
