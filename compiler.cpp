#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <cstdio>
#include <cstring>

#include "LineParser.h"
#include "Utility.h"
using namespace std;

void cosoleInputOutput();
void fileInputOutput();
void showInstruction();
// #define TABLE_SIZE 16

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
    // SymbolInfo *hashTable[TABLE_SIZE];
    SymbolInfo **hashTable;
    ScopeTable *child; // for maintaining child, like next link
    ScopeTable *parent; // for maintaining parent, like prev link
    int TABLE_SIZE;

    ScopeTable(int n)
    {  
        TABLE_SIZE = n;
        hashTable = new SymbolInfo*[TABLE_SIZE];

        for (int i = 0; i < n; i++)
        {
            hashTable[i] = NULL;
        }
        child = NULL; 
        parent = NULL;
        id = to_string(++tableCount);
    }

    ScopeTable *getRoot()
    {
        ScopeTable *cursor;
        cursor = this;
        while (cursor->parent != NULL)
        {
            cursor = cursor->parent;
        }
        
        return cursor;
    }

    string getId()
    {
        return id;
    }

    string getParentId()
    {
        ScopeTable *cursor;
        cursor = getRoot();

        string longId;

        while (cursor != this)
        {
            longId += cursor->id + ".";
            cursor = cursor->child;
        }

        return longId;
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
        cout<<"# scope table id = "<< getParentId() + getId() <<endl;
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
    int option;

    cout<<"# ============ MENU =========="<<endl;
    cout<<"# Enter '1' for Console Input"<<endl;
    cout<<"# Enter '2' for File Input"<<endl;
    cout<<"# Enter '3' for Exit Program"<<endl;
    cout<<"# ============================"<<endl;
    cin>> option;

    if(option == 1)
        cosoleInputOutput();
    else if (option == 2)
        fileInputOutput();
    else if (option == 3)
        exit(0);

    return 0;
}


// =======================================================
// ================ Reading From Console =================
// =======================================================
void cosoleInputOutput()
{
    showInstruction();      

    int m;
    cin>>m;
    cin.ignore();

    string text;
    int choice;

    SymbolInfo *symbolInfo = NULL;

    ScopeTable *currentScopeTable = NULL;

    SymbolTable *symbolTable = NULL;

    currentScopeTable = new ScopeTable(m);

    symbolTable = new SymbolTable();
    
    symbolTable->append(currentScopeTable); // appending as 1st node of doubly linked list

    fflush(stdin);
    while(getline(cin, text))
    {
        LineParser<string> parser(text, ' ');
        auto words = parser.parseLineToString();
        choice = to_ascii(words.front());

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

            string scopeNo = currentScopeTable->getParentId() + currentScopeTable->getId();
            if(currentScopeTable->lookup(name) != NULL)
            {
                cout<<name<<" found in scope table "<< scopeNo <<endl;
                
                // resetting currentScope, limiting scope
                {
                    ScopeTable *root;
                    ScopeTable *cursor;
                    root = cursor = symbolTable->getHead();

                    if(currentScopeTable == root)
                        break;

                    while (cursor->child != NULL)
                    {
                        cursor = cursor->child;
                    }

                    currentScopeTable = cursor;
                    cursor->child = NULL;
                }
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
            currentScopeTable = new ScopeTable(m);
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
            cursor->child = NULL;

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
}


// =======================================================
// ================= Reading From File ===================
// =======================================================
void fileInputOutput()
{
    showInstruction();
    fstream fin;
    string filleName;
    string text;
    int m;
    int choice;

    cout<<"# Enter file name: ";
    cin>>filleName;

    fin.open(filleName); // opening file
    getline(fin, text); // reading first line

    m = atoi(text.c_str()); // extracting m

    SymbolInfo *symbolInfo = NULL;

    ScopeTable *currentScopeTable = NULL;

    SymbolTable *symbolTable = NULL;

    currentScopeTable = new ScopeTable(m);

    symbolTable = new SymbolTable();
    
    symbolTable->append(currentScopeTable); // appending as 1st node of doubly linked list

    fflush(stdin);
    while(getline(fin, text))
    {
        LineParser<string> parser(text, ' ');
        auto words = parser.parseLineToString();
        choice = to_ascii(words.front());

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

            string scopeNo = currentScopeTable->getParentId() + currentScopeTable->getId();
            if(currentScopeTable->lookup(name) != NULL)
            {
                cout<<name<<" found in scope table "<< scopeNo <<endl;
                
                // resetting currentScope, limiting scope
                {
                    ScopeTable *root;
                    ScopeTable *cursor;
                    root = cursor = symbolTable->getHead();

                    if(currentScopeTable == root)
                        break;

                    while (cursor->child != NULL)
                    {
                        cursor = cursor->child;
                    }

                    currentScopeTable = cursor;
                    cursor->child = NULL;
                }
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
            currentScopeTable = new ScopeTable(m);
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
            cursor->child = NULL;

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
}

void showInstruction()
{
    cout<<"\t# Instructions"<<endl;
    cout<<"\tI for Insert"<<endl;
    cout<<"\tL for Lookup"<<endl;
    cout<<"\tD for Delete"<<endl;
    cout<<"\tP for Print"<<endl;
    cout<<"\tS for New Scope"<<endl;
    cout<<"\tE for Exit Current Scope"<<endl;
    cout<<"\tA for All"<<endl;
    cout<<"\tC for Current"<<endl;
    cout<<endl;
}