/**
 * @file compiler.cpp
 * @author Zahidul Islam (link)
 * @brief 
 * @version 0.1
 * @date 2022-05-29
 * 
 * @copyright Copyright (c) 2022
 * 
 * # for reading from file provide filename as command line argument
 * 
 * # Instructions
 * # I for Insert
 * # L for Lookup
 * # D for Delete
 * # P for Print
 * # S for New Scope
 * # E for Exit Current Scope
 * # A for All
 * # C for Current
 * 
 */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <cstdio>
#include <cstring>

#include "LineParser.h"
#include "Utility.h"
using namespace std;

int idTracker[32] = {0}; // let's assume that the doubly linked list size will not exceed 32! 
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
        string longId;

        if (tableCount == 1)
        {
            return to_string(tableCount);
        }
        else if (tableCount == 2)
        {
            return to_string(1) + "." + to_string(1);
        }
        else if (tableCount == 3)
        {
            return to_string(1) + "." + to_string(1) + "." + to_string(1);
        }
        

        int i=0;
        while (true)
        {
            if(idTracker[i+1] == 0)
            {
                longId += to_string(idTracker[i]);
                break;
            }
            longId += to_string(idTracker[i]) + ".";
            i++;
        }
        
        return longId;
    }


    bool insert(SymbolInfo *symbolInfo)
    {
        string symbolName = symbolInfo->getName();
        int index = hashf(symbolName, TABLE_SIZE);

        if(hashTable[index] == NULL) 
        {
            symbolInfo->next = hashTable[index];
            hashTable[index] = symbolInfo;


            string scopeNo = getId();
            cout<<"Inserted in ScopeTable #"<< scopeNo <<" at position "<< index <<", 0"<<endl;
        
            return true;
        }
        else
        {
            int chainNo = 0;
            SymbolInfo *cursor; 
            cursor = hashTable[index]; // getting the head of the current chain
            while (cursor->next != NULL)
            {
                string currentName = cursor->getName();
                if(currentName.compare(symbolName) == 0)
                {
                    cout<< symbolName <<" already exists in current ScopeTable"<<endl;
            
                    return false;
                }
                cursor = cursor->next;
                chainNo++;
            }
            cursor->next = symbolInfo;
            string scopeNo = getId();
            cout<<"Inserted in ScopeTable #"<< scopeNo <<" at position "<< index <<", "<< chainNo <<endl;
            return true;
        }
    }

    SymbolInfo *lookup(string name)
    {
        int index = hashf(name, TABLE_SIZE);
        string scopeNo = getId();

        if(hashTable[index] == NULL) 
        {
            cout<<"Not Found in ScopeTable #"<< scopeNo <<endl;
            return NULL;
        }

        SymbolInfo *cursor; 
        cursor = hashTable[index]; 
        int chainNo = 0;
        while (cursor != NULL)
        {
            string currentName = cursor->getName();
            if(currentName.compare(name) == 0)
            {
                cout<<"Found in ScopeTable #"<< scopeNo <<" at position "<< index <<", "<<chainNo<<endl;
        
                return cursor;
            }
            cursor = cursor->next;
            chainNo++;
        }

        return NULL;
    }

    bool remove(string name)
    {
        if(lookup(name) == NULL) 
        {
            cout<<name<<" could not be deleted from current ScopeTable"<<endl;
            return false;
        }
        int index = hashf(name, TABLE_SIZE);


        // for matching of 1st node
        SymbolInfo *cursor; 
        cursor = hashTable[index];
        string cursorName = cursor->getName();

        if(cursorName.compare(name) == 0)
        {
            hashTable[index] = cursor->next;
            cout<<"Deleted Entry "<< index <<", 0" <<" from current ScopeTable"<<endl;
            cursor = NULL;
            return true;
        }

        // for matching from 2nd node to last
        SymbolInfo *current, *prev; 
        current = hashTable[index];
        prev = hashTable[index]; 

        int chainNo = 0;
        while (current->next != NULL)
        {
            prev = current;
            current = current->next;
            chainNo++;

            string currentName = current->getName();
            if(currentName.compare(name) == 0)
            {
                prev->next = current->next;
                current->next = NULL;
                cout<<"Deleted Entry "<< index <<", 0"<< chainNo <<" from current ScopeTable"<<endl;

                return true;
            }
        }

        return true;
    }

    void print()
    {
        cout<<endl;
        cout<<"# scope table id = "<< this->getId() <<endl;
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

    int getSymbolTableLength()
    {
        int length = 0;
        ScopeTable *cursor;
        cursor = head;

        if (cursor == NULL)
        {
            return 1;
        }

        while (cursor->child != NULL)
        {
            cursor = cursor->child;
            length++;
        }
        return length;

    }

    ScopeTable *getHead() // head of doubly linked list
    {
        return head;
    }

    ScopeTable *getTail() // tail of doubly linked list
    {
        ScopeTable *cursor;
        cursor = head;
        while (cursor->child != NULL)
        {
            cursor = cursor->child;
        }
        return cursor;
    }

    void append(ScopeTable *newNode) // appedning new ScopeTable at the tail of doubly linked list (SymbolTable)
    {   
        int length;

        int count = 0;
        if(head == NULL)
        {
            head = newNode;

            string scopeNo = head->getId();
            cout<<"New ScopeTable with id "<< scopeNo <<" created"<<endl;
            
            length = getSymbolTableLength();
            idTracker[0] = 1;

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
        string scopeNo = newNode->getId();
        cout<<"New ScopeTable with id "<< scopeNo <<" created"<<endl;
        
        length = getSymbolTableLength();
        idTracker[length] += 1;
    }

    SymbolInfo *lookup(string name)
    {
        SymbolInfo *temp = NULL;
        ScopeTable *cursor;
        cursor = head;

        // searching in every ScopeTable
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
        ScopeTable *currentScopeTable = this->getTail();
        currentScopeTable->print();
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
    }

    ScopeTable *deleteCurrentScopeTable(ScopeTable *currentScopeTable)
    {
        ScopeTable *root;
        ScopeTable *cursor;
        root = cursor = this->getHead();

        // of current scope table is the only remaining scope table, then just delete it
        if(currentScopeTable == root)
        {
            // currentScopeTable = NULL; // deletion causing segmentation fault
            
            return currentScopeTable;
        }

        while (cursor->child != currentScopeTable)
        {
            cursor = cursor->child;
        }
        string scopeNo = currentScopeTable->getId();
        cout<<"ScopeTable with id "<< scopeNo <<" removed"<<endl;
        
        currentScopeTable = cursor;
        delete(cursor->child);
        cursor->child = NULL;

        return currentScopeTable;
    }

    ScopeTable *resetCurrentScopeTable()
    {
        // pass
        return NULL;
    }

};


// =======================================================
// ==================== Main Function ====================
// =======================================================
// accepts command line argument
// no command line argument = console i/o
// command line argument = file i/o
int main(int argc, char const *argv[])
{
    bool console = true;
    string fileName;
    int m; // bucket size
    fstream fin;
    string text;
    int choice;

    if(argc == 1)
    {
        cin>>m;
        cin.ignore();
    }
    else if(argc == 2)
    {
        fileName = argv[1];
        fin.open(fileName); // opening file
        getline(fin, text); // reading first line

        // m = atoi(text.c_str()); // extracting m
        m = stoi(text); // extracting m
        console = false;
    }

    SymbolInfo *symbolInfo = NULL;
    ScopeTable *currentScopeTable = NULL;
    SymbolTable *symbolTable = NULL;
    vector<string> words;

    currentScopeTable = new ScopeTable(m);
    symbolTable = new SymbolTable();
    symbolTable->append(currentScopeTable); // appending as 1st node of doubly linked list

    while(true)
    {
        if(console == true)
        {
            getline(cin, text);
            LineParser<string> parser(text, ' ');
            words = parser.parseLineToString();
            choice = to_ascii(words.front());
        }
        else if(console == false)
        {
            getline(fin, text);
            words = tokenizeText(text);
            choice = to_ascii(words.front());
            if(fin.eof() != 0) break;
        }


        switch (choice)
        {
        case I: // ascii value of I, insert
        {
            string name = words.at(1);
            string type = words.at(2);
            if(currentScopeTable->lookup(name) != NULL) break;

            symbolInfo = new SymbolInfo(name, type);
            currentScopeTable->insert(symbolInfo);
            break;
        }
        
        case L: // ascii value of L, lookup
        {
            string name;
            name = words.at(1);
            symbolTable->lookup(name);
            currentScopeTable = symbolTable->getTail();

            break;
        }
        
        case D: // ascii value of D, delete
        {
            string name;
            name = words.at(1);
            symbolTable->remove(name);
            currentScopeTable = symbolTable->getTail();

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
                // currentScopeTable->print();
                symbolTable->printCurrentScopeTable();
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
            currentScopeTable = symbolTable->deleteCurrentScopeTable(currentScopeTable);

            break;
        }
        
        case Q: // ascii value of Q, quit
            cout<<"# Quit Program" <<endl;
            cout<< "~TERMINATED" <<endl;
            exit(0);
            break;
            
        case NEWLINE:
        case SPACE:
            continue;
        
        default:
            break;
        }
    }
    
    fin.close();

    return 0;
}