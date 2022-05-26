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
    static int tableCount;
    string id;
    SymbolInfo *hashTable[TABLE_SIZE];
    ScopeTable *child; // for maintaining child is like next link
    ScopeTable *parent; // for maintaining parent is like prev link

    ScopeTable()
    {
        for (size_t i = 0; i < TABLE_SIZE; i++)
        {
            hashTable[i] = NULL;
        }
        child = NULL;
        parent = NULL;
        id = to_string(++tableCount);
    }

    bool insert(SymbolInfo *symbolInfo)
    {
        string key = symbolInfo->getName();
        int index = hashf(key);
        // cout<<"# index = "<<index<<endl;

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

        return true;
    }

    SymbolInfo *lookup(string name)
    {
        int index = hashf(name);

        if(hashTable[index] == NULL) 
        {
            cout<< name <<" not found" <<endl;
            return NULL;
        }

        SymbolInfo *cursor; 
        cursor = hashTable[index]; 
        while (cursor != NULL)
        {
            string currentName = cursor->getName();
            if(currentName.compare(name) == 0)
            {
                cout<< name << " found" <<endl;
                return cursor;
            }
            cursor = cursor->next;
        }
        
        // pass
        return NULL;
    }

    bool remove(string name)
    {
        int index = hashf(name);

        if(hashTable[index] == NULL) 
        {
            return false;
        }

        // for matching of 1st node
        SymbolInfo *cursor; 
        cursor = hashTable[index];
        string cursorName = cursor->getName();
        if(cursorName.compare(name) == 0)
        {
            hashTable[index] = cursor->next;
            cursor = NULL;
            return true;
        }

        SymbolInfo *current, *prev; 
        current = hashTable[index];
        prev = hashTable[index]; 
        while (current->next != NULL)
        {
            prev = current;
            current = current->next;

            string currentName = current->getName();
            if(currentName.compare(name) == 0)
            {
                prev->next = current->next;
                current->next = NULL;
                return true;
            }
        }

        return true;
    }

    void print()
    {
        cout<<"# table id = "<< id <<endl;
        for(int i=0; i<TABLE_SIZE; i++)
        {
            if (hashTable[i] == NULL)
            {
                printf("\t%i\t---\n", i);
            }
            else
            {
                printf("\t%i\t",i);
                SymbolInfo *cursor = hashTable[i];
                while (cursor != NULL)
                {
                    string name = cursor->getName(); 
                    string type = cursor->getType(); 
                    printf("<%s,%s>  ", name.c_str(), type.c_str());
                    cursor = cursor->next;
                }
                printf("\n");   
            }
        }
        printf("\n");
    }

    // pass
};

int ScopeTable::tableCount = 0;

// linked list of multiple ScopeTable // make it doubly linked list
class SymbolTable
{
public:
    ScopeTable *head;

    SymbolTable()
    {
        head = NULL;
    }

    void append(ScopeTable *newNode)
    {
        if(head == NULL)
        {
            head = newNode;
            return;
        }

        ScopeTable *cursor;
        cursor = head;
        while (cursor->child != NULL)
        {
            cursor = cursor->child;
        }
        cursor->child = newNode;
        newNode->parent = cursor;
    }

    void printCurrentScopeTable()
    {
        // pass
    }

    void printAllScopeTable()
    {
        if(head == NULL)
        {
            return;
        }

        ScopeTable *cursor;
        cursor = head;
        while (cursor != NULL)
        {
            cursor->print();
            cursor = cursor->child;
        }
        // pass
    }
    // pass
};

int main()
{
    // =======================================================
    // ===================== READ FILE =======================
    // =======================================================

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

    // =======================================================
    // ========================== TEST =======================
    // =======================================================

/* 

    // cout<<hashf("int")<<endl;
    // cout<<hashf("float")<<endl;
    // cout<<hashf("main")<<endl;
    // cout<<hashf("printf")<<endl;
    // cout<<hashf("++")<<endl;
    // cout<<hashf("+")<<endl;
    // cout<<hashf("*")<<endl;
    // cout<<hashf("return")<<endl;

    ScopeTable scopeTable1;
    scopeTable1.print();

    SymbolInfo s1("int", "KEYWORD");
    SymbolInfo s2("float", "KEYWORD");
    SymbolInfo s3("printf", "FUNCTION");
    SymbolInfo s4("n", "IDENTIFIER");
    SymbolInfo s5("sum", "IDENTIFIER");
    SymbolInfo s6("int", "KEYWORD");
    SymbolInfo s7("++", "OPERATOR");
    SymbolInfo s8("--", "OPERATOR");
    SymbolInfo s9("&&", "OPERATOR");
    SymbolInfo s10("int", "IDENTIFIER");

    scopeTable1.insert(&s1);
    scopeTable1.insert(&s2);
    scopeTable1.insert(&s3);
    scopeTable1.insert(&s4);
    scopeTable1.insert(&s5);
    scopeTable1.insert(&s6);
    scopeTable1.insert(&s7);
    scopeTable1.insert(&s8);
    scopeTable1.insert(&s9);
    scopeTable1.insert(&s10);

    scopeTable1.print();

    cout<<scopeTable1.lookup("||")<<endl;
    cout<<scopeTable1.lookup("int")<<endl;
    cout<<scopeTable1.lookup("++")<<endl;

    scopeTable1.remove("&&");
    scopeTable1.remove("int");
    scopeTable1.remove("++");
    scopeTable1.remove("n");

    scopeTable1.print();

    cout<<endl<<endl;


    SymbolInfo t1("double", "KEYWORD");
    SymbolInfo t2("long", "KEYWORD");
    SymbolInfo t3("unisigned", "FUNCTION");
    SymbolInfo t4("x", "IDENTIFIER");
    SymbolInfo t5("count", "IDENTIFIER");
    SymbolInfo t6("int", "KEYWORD");
    SymbolInfo t7("*", "OPERATOR");
    SymbolInfo t8(">>", "OPERATOR");
    SymbolInfo t9("&&", "OPERATOR");
    SymbolInfo t10("int", "IDENTIFIER");
    
    ScopeTable scopeTable2;
    scopeTable2.print();

    scopeTable2.insert(&t1);
    scopeTable2.insert(&t2);
    scopeTable2.insert(&t3);
    scopeTable2.insert(&t4);
    scopeTable2.insert(&t5);
    scopeTable2.insert(&t6);
    scopeTable2.insert(&t7);
    scopeTable2.insert(&t8);
    scopeTable2.insert(&t9);
    scopeTable2.insert(&t10);

    scopeTable2.print();

    cout<<scopeTable2.lookup("||")<<endl;
    cout<<scopeTable2.lookup("int")<<endl;
    cout<<scopeTable2.lookup("++")<<endl;

    scopeTable2.remove("&&");
    scopeTable2.remove("int");
    scopeTable2.remove("++");
    scopeTable2.remove("n");

    scopeTable2.print();

    cout<<endl<<endl;


    SymbolTable symbolTable;
    symbolTable.append(&scopeTable1);
    symbolTable.append(&scopeTable2);
    symbolTable.printAllScopeTable(); */

    // =======================================================
    // ========================== MENU =======================
    // =======================================================

    string text; 
    int choice;

    int m; // bucket size
    // cin>>m;

    SymbolInfo *symbolInfo = NULL;
    ScopeTable *scopeTable = NULL;
    SymbolTable *symbolTable = NULL;

    scopeTable = new ScopeTable();
    symbolTable = new SymbolTable();
    
    symbolTable->append(scopeTable);

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
        {
            // cout<<"# Insert" <<endl;
            string name = words.at(1);
            string type = words.at(2);

            // cout<<"# name = "<< name <<endl;
            // cout<<"# type = "<< type <<endl;
            symbolInfo = new SymbolInfo(name, type);
            scopeTable->insert(symbolInfo);
            // pass
            break;
        }
        
        case L: // ascii value of L, lookup
        {

            cout<<"# Lookup" <<endl;
            string name;
            cin>>name;
            // pass
            break;
        }
        
        case D: // ascii value of D, delete
            cout<<"# Delete" <<endl;
            // pass
            break;
        
        case P: // ascii value of P, print symbol table
        {

            cout<<"# Print" <<endl;
            string instruction = words.at(1);
            if (instruction.compare("A") == 0)
            {
                symbolTable->printAllScopeTable();
            }
            else if (instruction.compare("C") == 0)
            {
                symbolTable->printCurrentScopeTable(); // yet to be implemented
            }
            
            // pass
            break;
        }
        
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
