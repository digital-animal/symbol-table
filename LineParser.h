#include<iostream>
#include<string>
#include<sstream>
#include<vector>

#include<cstring>
using namespace std;

template<typename T>
class LineParser
{
public:
    string str;
    char delim;
    vector<T> output;

public:
    LineParser()
    {
        // pass
    }

    LineParser(string line, char delimeter)
    {
        str = line;
        delim = delimeter;
    }

    vector<T> parseLineToInteger()
    {
        stringstream ss(str);
        string token;
        int num;

        while(getline(ss, token, delim))
        {
            num = stoi(token);
            output.push_back(num);
        }
        return output;
    }

    vector<T> parseLineToChar()
    {
        stringstream ss(str);
        string token;
        char ch;

        while(getline(ss, token, delim))
        {
            ch = token.c_str()[0];
            output.push_back(ch);
        }
        return output;
    }

    vector<T> parseLineToString()
    {
        stringstream ss(str);
        string token;

        while(getline(ss, token, delim))
        {
            output.push_back(token);
        }
        return output;
    }
};

