#ifndef HASH_TABLES
#define HASH_TABLES

#include <vector>
#include <string>
#include <cstdint>
#include <iostream>

#include "functions.h"

using namespace std;

const int MAX_ATTEMPTS = 1024;

/**
 * @brief Clase que representa una tabla hash para almacenar objetors User.
*/
class HashTable
{
public:
    int size; ///< Tamaño de la tabla hash.
    vector<User *> table; ///< Vector que almacena punteros a onjetos User.
    int (*hashing_method)(uint64_t, int, int); ///< Puntero a la función de hash.

    /**
     * @brief Constructor para inicializar la tabla hash con un tamaño dado y un método de hash.
     * @param size Tamaño de la tabla hash.
     * @param hashing_method Puntero a la función de hash que se usará.
    */
    HashTable(int size, int (*hashing_method)(uint64_t, int, int))
        : size(size), hashing_method(hashing_method), table(size, nullptr) {}

    /**
     * @brief Inserta un usuario en la tabla hash.
     * @param userId El ID del usuario a insertar.
     * @param user Puntero al objeto User que se va a insertar.
     */
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

    /**
     * @brief Busca un usuario en la tabla hash por su ID.
     * @param userId El ID del usuario a buscar.
     * @return Puntero al objeto User si se encuentra, nullptr en caso contrario.
     */
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

    /**
     * @brief Destructor para liberar la memoria asignada a los objetos User en la tabla hash.
     */
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

/**
 * @brief Clase que implementa una tabla hash utilizando encadenamiento (separate chaining) para la resolución de colisiones.
 */
class HashTableChaining
{
public:
    int size; ///< Tamaño de la tabla hash
    vector<vector<User *>> table; ///< Vector de vectores que representa la tabla hash con listas de encadenamiento

    /**
     * @brief Constructor de la clase HashTableChaining.
     * 
     * @param size El tamaño de la tabla hash.
     */
    HashTableChaining(int size) : size(size), table(size) {}

    /**
     * @brief Inserta un usuario en la tabla hash.
     * 
     * @param userId El ID del usuario a insertar.
     * @param user Un puntero al objeto User que contiene los datos del usuario.
     */
    void insert(uint64_t userId, User *user)
    {
        int index = userId % size;
        table[index].push_back(user);
    }

    /**
     * @brief Busca un usuario en la tabla hash por su ID.
     * 
     * @param userId El ID del usuario a buscar.
     * @return Un puntero al objeto User encontrado, o nullptr si no se encontró.
     */
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

    /**
     * @brief Destructor de la clase HashTableChaining. Libera la memoria de todos los usuarios almacenados.
     */
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