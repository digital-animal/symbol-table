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

// ==========================================
// class containing actual symbole/token info
// ==========================================
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


// ============================================
// hashmap of multiple SymbolInfo in same scope
// ============================================
class ScopeTable
{
public:
    static int tableCount;
    string id;
    SymbolInfo *hashTable[TABLE_SIZE];
    ScopeTable *child; // for maintaining child, like next link
    ScopeTable *parent; // for maintaining parent, like prev link

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
        int index = hashf(key, TABLE_SIZE);

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
        int index = hashf(name, TABLE_SIZE);

        if(hashTable[index] == NULL) 
        {
            return NULL;
        }

        SymbolInfo *cursor; 
        cursor = hashTable[index]; 
        while (cursor != NULL)
        {
            string currentName = cursor->getName();
            if(currentName.compare(name) == 0)
            {
                return cursor;
            }
            cursor = cursor->next;
        }

        return NULL;
    }

    bool remove(string name)
    {
        int index = hashf(name, TABLE_SIZE);

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
        cout<<"# scope table id = "<< id <<endl;
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
};

int ScopeTable::tableCount = 0;


// ============================================================
// linked list of multiple ScopeTable, using doubly linked list
// ============================================================
class SymbolTable
{
public:
    ScopeTable *head;

    SymbolTable()
    {
        head = NULL;
    }

    ScopeTable *getHead()
    {
        return head;
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

    SymbolInfo *lookup(string name)
    {
        SymbolInfo *temp = NULL;
        ScopeTable *cursor;
        cursor = head;

        while(cursor != NULL)
        {
            temp = cursor->lookup(name);
            if(temp != NULL) 
            {
                return temp;
            }
            cursor = cursor->child;
        }

        return NULL;
    }

    bool remove(string name)
    {
        bool removed = false;
        ScopeTable *cursor;
        cursor = head;

        while(cursor != NULL)
        {
            removed = cursor->remove(name);
            if(removed == true)
            {
                break;
            }
            cursor = cursor->child;
        }

        return removed;
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
    // ========================== MENU =======================
    // =======================================================

    string text; 
    int choice;

    int m; // bucket size
    // cin>>m;

    SymbolInfo *symbolInfo = NULL;

    ScopeTable *currentScopeTable = NULL;

    SymbolTable *symbolTable = NULL;

    currentScopeTable = new ScopeTable();

    symbolTable = new SymbolTable();
    
    symbolTable->append(currentScopeTable); // appending as 1st node of doubly linked list

    while(getline(cin, text))
    {
        
        LineParser<string> parser(text, ' ');
        auto words = parser.parseLineToString();
        choice = to_ascii(words.front());
        
        // I for Insert
        // L for Lookup
        // D for Delete
        // P for Print
        // S for New Scope
        // E for Exit Current Scope
        // A for All
        // C for Current
        // Q for Quit

        switch (choice)
        {
        case I: // ascii value of I, insert
        {
            string name = words.at(1);
            string type = words.at(2);

            symbolInfo = new SymbolInfo(name, type);
            currentScopeTable->insert(symbolInfo);
            break;
        }
        
        case L: // ascii value of L, lookup
        {
        L1: // primitive way
            string name;
            name = words.at(1);

            string scopeNo = currentScopeTable->id;
            if(currentScopeTable->lookup(name) != NULL)
            {
                cout<<name<<" found in scope table "<< scopeNo <<endl;
                break;
            }
            cout<<name<<" not found in scope table "<< scopeNo <<endl;

            ScopeTable *root;
            ScopeTable *cursor;
            root = cursor = symbolTable->getHead();

            if(currentScopeTable == root)
                break;

            while (cursor->child != currentScopeTable)
            {
                cursor = cursor->child;
            }

            currentScopeTable = cursor;
            
            choice = L;
            goto L1;

            break;
        }
        
        case D: // ascii value of D, delete
        {
            string name;
            name = words.at(1);
            if(currentScopeTable->remove(name) == true) 
                cout<<name<<" DELETED"<<endl;
            else
                cout<<name<<" NOT DELETED"<<endl;

            break;
        }
        
        case P: // ascii value of P, print symbol table
        {
            string instruction = words.at(1);
            if (instruction.compare("A") == 0)
            {
                symbolTable->printAllScopeTable();
            }
            else if (instruction.compare("C") == 0)
            {
                currentScopeTable->print();
            }
            
            break;
        }
        
        case S: // ascii value of S, entering new scope
        {
            currentScopeTable = new ScopeTable();
            symbolTable->append(currentScopeTable);

            break;
        }
        
        case E: // ascii value of E, entering current scope
        {
            ScopeTable *root;
            ScopeTable *cursor;
            root = cursor = symbolTable->getHead();

            if(currentScopeTable == root)
                break;

            while (cursor->child != currentScopeTable)
            {
                cursor = cursor->child;
            }

            currentScopeTable = cursor;
            break;
        }
        
        case Q: // ascii value of Q, quit
            cout<<"# Quit Program" <<endl;
            cout<< "~TERMINATED" <<endl;
            exit(0);
            break;
        
        default:
            break;
        }
        
    }


    return 0;
}
