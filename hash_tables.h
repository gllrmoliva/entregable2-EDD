#ifndef HASH_TABLES
#define HASH_TABLES

#include <vector>
#include <string>
#include <cstdint>
#include <iostream>

#include "functions.h"

using namespace std;

const int MAX_ATTEMPTS = 100;

class HashTable
{
public:
    int size;
    vector<User *> table;
    int (*hashing_method)(uint64_t, int, int);

    HashTable(int size, int (*hashing_method)(uint64_t, int, int))
        : size(size), hashing_method(hashing_method), table(size, nullptr) {}

    void insert(User *user)
    {
        int i = 0;
        int index;
        do
        {
            index = hashing_method(user->userId, size, i);
            if (table[index] == nullptr)
            {
                table[index] = user;
                return;
            }
            i++;
        } while (i < size);
        cout << "Error: Hash table overflow" << endl;
    }

    User *search(uint64_t userId)
    {
        int i = 0;
        int index;
        do
        {
            index = hashing_method(userId, size, i);
            if (table[index] == nullptr)
                return nullptr;
            if (table[index]->userId == userId)
                return table[index];
            i++;
        } while (i < size);
        return nullptr;
    }
    ~HashTable()
    {
        for (auto user : table)
        {
            delete user;
        }
    }
};
//-------------FUNCIONES DE HASHEO PARA KEY USERNAME----------------//

//------ HASH TABLE USER NAME--------//
// TODO: discutir si reutilizar las funciones hash ya propuestas o si crear unas unicas
// para el caso de
class HashTableUserName
{
public:
    int size;
    User **table; // Notemos que esto es una tabla con punteros de struct User
    int (*hashing_method)(const string &, int, int);

    HashTableUserName(int size, int (*hashing_method)(const string &, int, int))
        : size(size), hashing_method(hashing_method)
    {
        table = new User *[size];
        for (int i = 0; i < size; i++)
        {
            table[i] = nullptr;
        }
    }

    ~HashTableUserName()
    {
        delete[] table;
    }

    void insert(const string &key, User *user_data)
    {
        int i = 0;
        unsigned int index = hashing_method(key, size, i);
        while (table[index] != nullptr && (table[index]->userName != "DELETED_VAR") && i <= MAX_ATTEMPTS)
        {
            i++;
            index = hashing_method(key, size, i);
        }
        if (i <= MAX_ATTEMPTS)
            table[index] = user_data;
    }

    User *search(const string &key)
    {
        int i = 0;
        unsigned int index = hashing_method(key, size, i);
        while (table[index] != nullptr && table[index]->userName != key && i <= MAX_ATTEMPTS)
        {
            i++;
            index = hashing_method(key, size, i);
        }
        if (table[index] != nullptr && table[index]->userName == key && i <= MAX_ATTEMPTS)
        {
            cout << "Encontrado: " << endl;
            cout << table[index]->userName << endl;
            cout << table[index]->userId << endl;
            cout << table[index]->university << endl;
            cout << "indice en tabla hash: " << index << endl;

            return table[index];
        }
        cout << "No se encontro el usuario" << endl;
        return nullptr; // No se encontró el usuario
    }

    void remove(const string &key)
    {
        int i = 0;
        unsigned int index = hashing_method(key, size, i);
        while (table[index] != nullptr && table[index]->userName != key)
        {
            i++;
            index = hashing_method(key, size, i);
        }
        if (table[index] != nullptr && table[index]->userName == key)
        {
            // Se usa un usuario con el nombre DELETED_VAR
            User *DELETED_VAR = new User("", 0, "DELETED_VAR", 0, 0, 0, "");
            table[index] = DELETED_VAR;
        }
    }
};

//---------------FUNCIONES TEST--------------------//

// Tablaa hash con separate chaining (encadenamiento)
class HashTableChaining
{
public:
    int size;
    vector<vector<User *>> table;

    HashTableChaining(int size) : size(size), table(size) {}

    void insert(User *user)
    {
        int index = user->userId % size;
        table[index].push_back(user);
    }

    User *search(uint64_t userId)
    {
        int index = userId % size;
        for (User *user : table[index])
        {
            if (user->userId == userId)
                return user;
        }
        return nullptr;
    }

    ~HashTableChaining()
    {
        for (auto &bucket : table)
        {
            for (auto user : bucket)
            {
                delete user;
            }
        }
    }
};

#endif