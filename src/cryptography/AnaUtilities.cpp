#include"AnaUtilities.h"
#include"../base/format.h"
#include"../base/StringHandle.h"
#include"spn.h"
#include<regex>
#include<vector>
#include<fstream>
#include<sstream>
#include<bitset>
#include<cmath>
#include<cstdlib>

using namespace std;

void Analysis(string command)
{
    regex spnDic_regex("-spn-d\".*?\"");
    regex plainPath_regex("-p\".*?\"");
    regex cipherPath_regex("-c\".*?\"");
    regex num_regex("\\s-n=[0-9]+");
    regex algorithm_regex("\\s-[a-zA-Z]*?($|(?=\\s))");

    string spnDic;
    string plainPath;
    string cipherPath;
    int num = -1;
    string algorithm;

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
    else
    {
        spnDic = SPN::DefaultSpnPath;
    }

    if(regex_search(command, reg_smatch, plainPath_regex))
    {
        plainPath = reg_smatch.str().substr(3, reg_smatch.str().length() - 4);
    }
    else
    {
        cout << "No plain path, use -p\"...\"" << endl;
        cout << "See help for more help\n" << endl;
        return;
    }

    if(regex_search(command, reg_smatch, cipherPath_regex))
    {
        cipherPath = reg_smatch.str().substr(3, reg_smatch.str().length() - 4);
    }
    else
    {
        cout << "No cipher path, use -p\"...\"" << endl;
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
    if(num == -1)
    {
        cout << "No specified number\n" << endl;
        return;
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
        cout << "No algorithm, use -lin, -dif, etc" << endl;
        cout << "See help for more help\n" << endl;
        return;
    }

    if(algorithm == "lin")
    {
        SPNLinAnalysis(spnDic, plainPath, cipherPath, num);
        cout << endl;
    }
    else if(algorithm == "dif")
    {
        SPNDifAnalysis(spnDic, plainPath, cipherPath, num);
        cout << endl;
    }
    else
    {
        cout << "Unknown analysis algorithm: " << algorithm << "\n" << endl;
        return;
    }
}

// Forget it, the design of this class is not designed to expand.
//void SPNLinAnalysis(string spnDic, string plainPath, string cipherPath, int num)
//{
//    vector<int>x;
//    vector<int>y;
//    vector<int>u;
//
//    SPN::Load(spnDic);
//
//    string lineString;
//    stringstream stringStream;
//    ifstream fileStream;
//
//    fileStream.open(spnDic + "lin.txt");
//    if(!fileStream.is_open())
//    {
//        cout << "Lin analysis configure file can not open" << endl;
//        cout << "This SPN Cipher constitution does not support analysis" << endl;
//        return;
//    }
//
//    int temp;
//    /****** x read ******/
//    getline(fileStream, lineString);
//    stringStream << lineString;
//    while(stringStream >> temp)
//    {
//        x.push_back(temp);
//    }
//
//    /****** y read ******/
//    getline(fileStream, lineString);
//    stringStream << lineString;
//    while(stringStream >> temp)
//    {
//        y.push_back(temp);
//    }
//
//    /****** u read ******/
//    getline(fileStream, lineString);
//    stringStream << lineString;
//    while(stringStream >> temp)
//    {
//        u.push_back(temp);
//    }
//
//
//}


// Process oriented
void SPNLinAnalysis(string spnDic, string plainPath, string cipherPath, int num)
{
    int Count[16][16];

    memset(Count, 0, sizeof(Count));

    SPN::Load(SPN::DefaultSpnPath);     // Only support default

    ifstream plainsfile;
    ifstream cipherfile;

    plainsfile.open(plainPath);
    if(!plainsfile.is_open())
    {
        cout << "Can not open plainsfile" << endl;
        return;
    }

    cipherfile.open(cipherPath);
    if(!cipherfile.is_open())
    {
        cout << "Can not open ciphersfile" << endl;
        return;
    }

    bitset<16> x,y,v,u;
    bitset<16> k;
    bitset<4> t2, t3;
    string u_string;
    string v_string;
    string e;
    int z;

    float progress = 0;
    float lastProgress = -1;

    for(int n = 0; n < num; n++)
    {
        string plaintext;
        string ciphertext;

        getline(plainsfile, plaintext);
        getline(cipherfile, ciphertext);

        x = bitset<16>(bairuo::HexaToBinaryString(plaintext));
        y = bitset<16>(bairuo::HexaToBinaryString(ciphertext));

        progress = 1.0f * n / num * 100;
        if(progress - lastProgress > 1.5f)
        {
            lastProgress = progress;
            cout << "Progress: " << lastProgress << "%" << endl;
        }

        for(int i = 0; i < 16; i++)
        {
            for(int j = 0; j < 16; j++)
            {
                t2 = i;
                t3 = j;
                k = bitset<16>(e + "0000" + t2.to_string() + "0000" + t3.to_string());
                v = k ^ y;
                v_string = v.to_string();

                u_string = e + "0000" + SPN::sboxs[3].InverseOutput(v_string.substr(4, 4))
                           + "0000" + SPN::sboxs[3].InverseOutput(v_string.substr(12, 4));
                u = bitset<16>(u_string);

                z = x[11] ^ x[9] ^ x[8] ^ u[10] ^ u[8] ^ u[2] ^ u[0];


                if(z == 0)
                {
                    Count[i][j]++;
                }
            }
        }
    }

    plainsfile.close();
    cipherfile.close();

    int maxCount = -1;
    int maxi, maxj;

    for(int i = 0; i < 16; i++)
    {
        for(int j = 0; j < 16; j++)
        {
            Count[i][j] = abs(Count[i][j] - num / 2);

            if(Count[i][j] > maxCount)
            {
                maxCount = Count[i][j];
                maxi = i;
                maxj = j;
            }
        }
    }

    t2 = maxi;
    t3 = maxj;

    string maxkey1 = e + "**** " + t2.to_string() + " **** " + t3.to_string();
    string maxkey2 = e + "*" + bairuo::BinaryToHexaString(t2.to_string()) + "*" + bairuo::BinaryToHexaString(t3.to_string());

    cout << "Binary: " << maxkey1 << endl;
    cout << "Hexa:   " << maxkey2 << endl;
}

void SPNDifAnalysis(string spnDic, string plainPath, string cipherPath, int num)
{
    vector<string> plaingroup;
    vector<string> ciphergroup;
    int Count[16][16];

    memset(Count, 0, sizeof(Count));

    SPN::Load(SPN::DefaultSpnPath);     // Only support default

    ifstream plainsfile;
    ifstream cipherfile;

    plainsfile.open(plainPath);
    if(!plainsfile.is_open())
    {
        cout << "Can not open plainsfile" << endl;
        return;
    }

    cipherfile.open(cipherPath);
    if(!cipherfile.is_open())
    {
        cout << "Can not open ciphersfile" << endl;
        return;
    }

    bitset<16> dif("0000101100000000");
    bitset<16> x1, x2;
    bitset<16> y1, y2;
    bitset<16> v1, v2;
    bitset<16> k;
    bitset<4> y11, y12, y21, y22;
    bitset<4> u2, u4;
    bitset<4> u21, u22, u41, u42;
    bitset<4> t2, t3;
    string u1_string, u2_string;
    string v1_string, v2_string;
    string e;

    float progress = 0;
    float lastProgress = -1;

    int effectnum = 1;

    for(int i = 0; i < num; i++)
    {
        string plaintext;
        string ciphertext;

        getline(plainsfile, plaintext);
        getline(cipherfile, ciphertext);

        plaingroup.push_back(plaintext);
        ciphergroup.push_back(ciphertext);
    }

    plainsfile.close();
    cipherfile.close();

    for(int n = 0; n < num; n++)
    {
        string ciphertext1;
        string ciphertext2;

        ciphertext1 = ciphergroup[n];
        y1 = bitset<16>(bairuo::HexaToBinaryString(ciphertext1));
        y11 = bairuo::SubBit<4, 16>(y1, 0, 4);
        y21 = bairuo::SubBit<4, 16>(y1, 8, 4);

        string plaintext1 = plaingroup[n];
        x1 = bitset<16>(bairuo::HexaToBinaryString(plaintext1));
        x2 = x1 ^ dif;

        ciphertext2 = SPN::Encrypt(bairuo::BinaryToHexaString(x2.to_string()));
        y2 = bitset<16>(ciphertext2);
        y12 = bairuo::SubBit<4, 16>(y2, 0, 4);
        y22 = bairuo::SubBit<4, 16>(y2, 8, 4);

        progress = 1.0f * n / num * 100;
        if(progress - lastProgress > 1.5f)
        {
            lastProgress = progress;
            cout << "Progress: " << lastProgress << "%" << endl;
        }

        if(y11 == y12 && y21 == y22)
        {
            effectnum++;
            for(int i = 0; i < 16; i++)
            {
                for(int j = 0; j < 16; j++)
                {
                    t2 = i;
                    t3 = j;
                    k = bitset<16>(e + "0000" + t2.to_string() + "0000" + t3.to_string());

                    v1 = k ^ y1;
                    v2 = k ^ y2;
                    v1_string = v1.to_string();
                    v2_string = v2.to_string();

                    u21 = bitset<4>(SPN::sboxs[3].InverseOutput(v1_string.substr(4, 4)));
                    u22 = bitset<4>(SPN::sboxs[3].InverseOutput(v2_string.substr(4, 4)));
                    u41 = bitset<4>(SPN::sboxs[3].InverseOutput(v1_string.substr(12, 4)));
                    u42 = bitset<4>(SPN::sboxs[3].InverseOutput(v2_string.substr(12, 4)));

                    u2 = u21 ^ u22;
                    u4 = u41 ^ u42;

                    if(u2 == 6 && u4 == 6)
                    {
                        Count[i][j]++;
                    }
                }
            }
        }
    }

    int maxCount = -1;
    int maxi, maxj;

    //cout << "Effect Num: " << effectnum << endl;
    for(int i = 0; i < 16; i++)
    {
        for(int j = 0; j < 16; j++)
        {
            if(Count[i][j] > maxCount)
            {
                maxCount = Count[i][j];
                maxi = i;
                maxj = j;
            }
        }
    }
    t2 = maxi;
    t3 = maxj;

    string maxkey1 = e + "**** " + t2.to_string() + " **** " + t3.to_string();
    string maxkey2 = e + "*" + bairuo::BinaryToHexaString(t2.to_string()) + "*" + bairuo::BinaryToHexaString(t3.to_string());

    cout << "Binary: " << maxkey1 << endl;
    cout << "Hexa:   " << maxkey2 << endl;
}
