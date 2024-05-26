#ifndef HASH_TABLES
#define HASH_TABLES

#include <vector>
#include <string>
#include <cstdint>
#include <iostream>

#include "functions.h"

using namespace std;

const int MAX_ATTEMPTS = 1024;

class HashTable
{
public:
    int size;
    vector<User *> table;
    int (*hashing_method)(uint64_t, int, int);

    HashTable(int size, int (*hashing_method)(uint64_t, int, int))
        : size(size), hashing_method(hashing_method), table(size, nullptr) {}

    void insert(uint64_t userId, User *user)
    {
        int i = 0;
        int index;
        do
        {
            index = hashing_method(userId, size, i);
            if (table[index] == nullptr)
            {
                table[index] = user;
                return;
            }
            i++;
        } while (i < size);
        std::cout << "Error: Hash table overflow" << endl;
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
//-------------TABLAS DE HASHEO PARA KEY USERNAME----------------//
class CloseHashTableUserName
{
public:
    int size;
    User **table; // Notemos que esto es una tabla con punteros de struct User
    int (*hashing_method)(const string &, int, int);

    CloseHashTableUserName(int size, int (*hashing_method)(const string &, int, int))
        : size(size), hashing_method(hashing_method)
    {
        table = new User *[size];
        for (int i = 0; i < size; i++)
        {
            table[i] = nullptr;
        }
    }

    ~CloseHashTableUserName()
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
            std::cout << "Indice en tabla hash: " << index << endl;
            printUser(table[index]);
            std::cout << endl;

            return table[index];
        }
        std::cout << "No se encontro el usuario" << endl;
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

class OpenHashTableUserName
{
public:
    int size;
    vector<vector<User>> table; // Tabla de vectores de User

    OpenHashTableUserName(int size) : size(size), table(size)
    {
    }

    void insert(const string &key, const User &user_data)
    {
        unsigned int index = hashing_method(key);
        table[index].push_back(user_data);
    }

    User *search(const string &key)
    {
        unsigned int index = hashing_method(key);
        for (User &user : table[index])
        {
            if (user.userName == key)
            {
                std::cout << "INDICE: " << index << endl;
                printUser(&user);
                std::cout << endl;
                return &user;
            }
        }
        return nullptr;
    }

    void remove(const string &key)
    {
        unsigned int index = hashing_method(key);
        auto &bucket = table[index];
        int bucket_size = bucket.size();

        for (int i = 0; i < bucket_size; i++)
        {
            if (bucket.at(i).userName == key)
            {
                // Esto es iniciar el iterador y moverlo hasta el indice correspondiente
                bucket.erase(bucket.begin() + i);
                return;
            }
        }
    }

private:
    unsigned int hashing_method(const string &key)
    {
        return linear_probing_usename(key, size, 0);
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

    void insert(uint64_t userId, User *user)
    {
        int index = userId % size;
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