#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <cstring>
#include <cstdio>
#include <cstring>

#include "LineParser.h"
#include "Utility.h"
using namespace std;

#define TABLE_SIZE 16

// contains actual symbole/token info
class SymbolInfo 
{
private:
    string name;
    string type;
public:
    SymbolInfo *next; // SymbolInfo next pointer for chaining

    SymbolInfo()
    {
        // pass
    }

    SymbolInfo(string name, string type)
    {
        setName(name);
        setType(type);
        this->next = NULL;
    }

    void setName(string name)
    {
        this->name = name;
    }

    void setType(string type)
    {
        this->type = type;
    }

    string getName()
    {
        return this->name;
    }

    string getType()
    {
        return this->type;
    }
};


int hashf(string key)
{
    return to_ascii(key) % TABLE_SIZE;
}


// hashmap of multiple SymbolInfo in same scope
class ScopeTable
{
public:
    int id;
    SymbolInfo *hashTable[TABLE_SIZE];
    ScopeTable *parentScope; // for maintaining parent is like prev link

    ScopeTable()
    {
        for (size_t i = 0; i < TABLE_SIZE; i++)
        {
            hashTable[i] = NULL;
        }
    }

    bool insert(SymbolInfo *symbolInfo)
    {
        string key = symbolInfo->getName();
        int index = hashf(key);
        cout<<"# index = "<<index<<endl;

        if(hashTable[index] == NULL) 
        {
            symbolInfo->next = hashTable[index];
            hashTable[index] = symbolInfo;
        }
        else
        {
            SymbolInfo *cursor; 
            cursor = hashTable[index]; 
            while (cursor->next != NULL)
            {
                cursor = cursor->next;
            }
            cursor->next = symbolInfo;
            
        }
        

        // symbolInfo->next = hashTable[index];
        // hashTable[index] = symbolInfo;

        return true;
    }

    SymbolInfo *lookup(string name)
    {
        // pass
        return NULL;
    }

    bool remove(string name)
    {
        // pass
        return true;
    }

    void print()
    {
        for(int i=0; i<TABLE_SIZE; i++)
        {
            if (hashTable[i] == NULL)
            {
                printf("\t%i\t---\n", i);
            }
            else
            {
                // printf("\t%i\t%s\n",i, hash_table[i]->name);
                printf("\t%i\t",i);
                SymbolInfo *cursor = hashTable[i];
                while (cursor != NULL)
                {
                    string name = cursor->getName(); 
                    printf("%s - ", name.c_str());
                    cursor = cursor->next;
                }
                printf("\n");   
            }
        }
        printf("\n");
    }

    // pass
};

// linked list of multiple ScopeTable
class SymbolTable
{
    // pass
};

int main()
{

// reading from file
/*     fstream fin;
    string filleName;
    string text;

    cout<<" # Enter file name: ";
    fflush(stdin);
    getline(cin, filleName);
    fin.open(filleName, ios::in);

    int i = 0;
    while(getline(fin, text))
    {
        LineParser<string> parser(text, ' ');
        auto words = parser.parseLineToString();
        for(auto word: words)
            cout<<word;
        i++;
    }
    fin.close();
    cout<<endl; */

    cout<<hashf("int")<<endl;
    cout<<hashf("float")<<endl;
    cout<<hashf("main")<<endl;
    cout<<hashf("printf")<<endl;
    cout<<hashf("++")<<endl;
    cout<<hashf("+")<<endl;
    cout<<hashf("*")<<endl;
    cout<<hashf("return")<<endl;

    SymbolInfo s1("int", "KEYWORD");
    SymbolInfo s2("float", "KEYWORD");
    SymbolInfo s3("printf", "FUNCTION");
    SymbolInfo s4("n", "IDENTIFIER");
    SymbolInfo s5("sum", "IDENTIFIER");
    SymbolInfo s6("int", "KEYWORD");

    ScopeTable scopeTable;
    scopeTable.insert(&s1);
    scopeTable.insert(&s2);
    scopeTable.insert(&s3);
    scopeTable.insert(&s4);
    scopeTable.insert(&s5);
    scopeTable.insert(&s6);

    scopeTable.print();


    // =======================================================
    // ========================== MENU =======================
    // =======================================================

    string text; 
    int choice;

    int m; // bucket size
    // cin>>m;
    while(getline(cin, text))
    {
        LineParser<string> parser(text, ' ');
        auto words = parser.parseLineToString();
        choice = to_ascii(words.front());
        
        // ascii values of choices
        // I = 73
        // L = 76
        // D = 68
        // P = 80
        // S = 83
        // E = 69
        // A = 65
        // C = 67
        // Q = 81

        switch (choice)
        {
        case I: // ascii value of I, insert
            cout<<"# Insert" <<endl;
            // pass
            break;
        
        case L: // ascii value of L, lookup
            cout<<"# Lookup" <<endl;
            // pass
            break;
        
        case D: // ascii value of D, delete
            cout<<"# Delete" <<endl;
            // pass
            break;
        
        case P: // ascii value of P, print symbol table
            cout<<"# Print" <<endl;
            // pass
            break;
        
        case S: // ascii value of S, entering new scope
            cout<<"# New Scope" <<endl;
            // pass
            break;
        
        case E: // ascii value of E, entering current scope
            cout<<"# Exit Scope" <<endl;
            // pass
            break;
        
        case Q: // ascii value of Q, quit
            cout<<"# Quit Program" <<endl;
            cout<< "~TERMINATED" <<endl;
            exit(0);
            // pass
            break;
        
        default:
            break;
        }
        
    }


    return 0;
}
