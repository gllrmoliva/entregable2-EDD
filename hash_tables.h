#ifndef HASH_TABLES
#define HASH_TABLES

#include <vector>
#include <string>
#include <cstdint>
#include <iostream>

#include "functions.h"
#include "hash_functions.h"

using namespace std;

// Máximo de intentos de una operación en una hash table.
const int MAX_ATTEMPTS = 50000;
User DELETED_VAR = User("", 0, "DELETED_VAR", 0, 0, 0, "");

//---------------------------------------------------------------//
//-------------TABLAS DE HASHEO PARA KEY USERID------------------//
//---------------------------------------------------------------//

/**
 * @brief Clase que representa una tabla hash para almacenar objetos User utilizando de key el parametro UserId
 */
class CloseHashTableUserId
{
public:
    int size;                                  ///< Tamaño de la tabla hash.
    vector<User *> table;                      ///< Vector que almacena punteros a onjetos User.
    int (*hashing_method)(uint64_t, int, int); ///< Puntero a la función de hash.

    /**
     * @brief Constructor para inicializar la tabla hash con un tamaño dado y un método de hash.
     * @param size Tamaño de la tabla hash.
     * @param hashing_method Puntero a la función de hash que se usará.
     */
    CloseHashTableUserId(int size, int (*hashing_method)(uint64_t, int, int))
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
};

/**
 * @brief Clase que implementa una tabla hash utilizando encadenamiento (separate chaining) para la resolución de colisiones.
 */
class OpenHashTableUserId
{
public:
    int max_size; ///< Tamaño de la tabla hash
    int size = 0;
    vector<vector<User *>> table; ///< Vector de vectores que representa la tabla hash con listas de encadenamiento

    /**
     * @brief Constructor de la clase HashTableChaining.
     *
     * @param size El tamaño de la tabla hash.
     */
    OpenHashTableUserId(int size) : max_size(size), table(size) {}

    /**
     * @brief Inserta un usuario en la tabla hash.
     *
     * @param userId El ID del usuario a insertar.
     * @param user Un puntero al objeto User que contiene los datos del usuario.
     */
    void insert(uint64_t userId, User *user)
    {
        unsigned int index = userId % max_size;
        table[index].push_back(user);
        size++;
    }

    /**
     * @brief Busca un usuario en la tabla hash por su ID.
     *
     * @param userId El ID del usuario a buscar.
     * @return Un puntero al objeto User encontrado, o nullptr si no se encontró.
     */
    User *search(uint64_t userId)
    {
        unsigned int index = userId % max_size;
        for (User *user : table[index])
        {
            if (user->userId == userId)
                return user;
        }
        return nullptr;
    }

    /**
     * @brief remueve un usuario en la tabla hash por su UserId, si este no existe no hace nada.
     *
     * @param key Id del usuario a buscar.
     */
    void remove(uint64_t key)
    {
        unsigned int index = key % max_size;
        auto &bucket = table[index];
        int bucket_size = bucket.size();

        for (int i = 0; i < bucket_size; i++)
        {
            if (bucket.at(i)->userId == key)
            {
                // Esto es iniciar el iterador y moverlo hasta el indice correspondiente
                bucket.erase(bucket.begin() + i);
                size--;
                return;
            }
        }
    }
};

//---------------------------------------------------------------//
//-------------TABLAS DE HASHEO PARA KEY USERNAME----------------//
//---------------------------------------------------------------//

/**
 * @brief Clase que representa una tabla hash para almacenar objetos User utilizando de key el parametro UserName
 */
class CloseHashTableUserName
{
public:
    int max_size;
    int size = 0;
    vector<User *> table;
    unsigned int (*hashing_method)(const string &, int, int);

    /**
     * @brief Constructor de la clase CloseHashUserName.
     *
     * @param size El tamaño de la tabla hash.
     * @param hashing_method Puntero a la función de hash que se usará.
     */
    CloseHashTableUserName(int size, unsigned int (*hashing_method)(const string &, int, int))
        : max_size(size), hashing_method(hashing_method), table(size, nullptr)
    {
    }

    /**
     * @brief Inserta un usuario en la tabla hash.
     *
     * @param key nombre del usuario a insertar.
     * @param user Un puntero al objeto User que contiene los datos del usuario.
     */
    void insert(const string &key, User *user_data)
    {
        int i = 0;
        unsigned int index = hashing_method(key, max_size, i);
        while (i < MAX_ATTEMPTS)
        {
            if (table[index] && table[index]->userName == key)
            {
                // cout << "La key ya esta en uso." << endl;
                return;
            }
            if (!table[index] || table[index]->userName == "DELETED_VAR")
            {
                table[index] = new User(*user_data);
                size++;
                return;
            }
            i++;
            index = hashing_method(key, max_size, i);
        }
        cout << "Tabla hash está llena o se alcanzó el máximo de intentos." << endl;
    }

    /**
     * @brief Busca un usuario en la tabla hash por su UserName.
     *
     * @param key nombre del usuario a buscar.
     * @return Un puntero al objeto User encontrado, o nullptr si no se encontró.
     */
    User *search(const string &key)
    {
        int i = 0;
        unsigned int index = hashing_method(key, max_size, i);
        while (i < MAX_ATTEMPTS && table[index])
        {
            if (table[index]->userName == key)
            {
                cout << "Indice en tabla hash: " << index << endl;
                printUser(table[index]);
                cout << endl;
                return table[index];
            }
            i++;
            index = hashing_method(key, max_size, i);
        }
        cout << "No se encontró el usuario" << endl;
        return nullptr;
    }

    /**
     * @brief remueve un usuario en la tabla hash por su UserName, si este no existe no hace nada.
     *
     * @param key nombre del usuario a buscar.
     */
    void remove(const string &key)
    {
        int i = 0;
        unsigned int index = hashing_method(key, max_size, i);
        while (i < MAX_ATTEMPTS && table[index])
        {
            if (!table[index])
            {
                // Esto pasa cuando nos encontramos con un espacio al cual nunca se ha accedido.
                return;
            }
            if (table[index]->userName == key)
            {
                table[index] = new User(DELETED_VAR);
                size--;
                return;
            }
            i++;
            index = hashing_method(key, max_size, i);
        }
    }
};

/**
 * @brief Clase que implementa una tabla hash con key User Name utilizando encadenamiento (separate chaining) para la resolución de colisiones.
 */
class OpenHashTableUserName
{
public:
    int max_size;
    int size = 0;
    vector<vector<User *>> table; // Tabla de vectores de User

    /**
     * @brief Constructor de la clase OpenHashTableUserName.
     *
     * @param size El tamaño de la tabla hash.
     */
    OpenHashTableUserName(int size) : max_size(size), table(size){};

    /**
     * @brief Inserta un usuario en la tabla hash.
     *
     * @param key nombre del usuario a insertar.
     * @param user Un puntero al objeto User que contiene los datos del usuario.
     */
    void insert(const string &key, User *user_data)
    {
        unsigned int index = hashing_method(key);
        table[index].push_back(user_data);
        size++;
    }

    /**
     * @brief Busca un usuario en la tabla hash por su UserName.
     *
     * @param key nombre del usuario a buscar.
     * @return Un puntero al objeto User encontrado, o nullptr si no se encontró.
     */
    User *search(const string &key)
    {
        unsigned int index = hashing_method(key);
        for (User *user : table[index])
        {
            if (user->userName == key)
            {
                std::cout << "INDICE: " << index << endl;
                printUser(user);
                std::cout << endl;
                return user;
            }
        }
        return nullptr;
    }

    /**
     * @brief remueve un usuario en la tabla hash por su UserName, si este no existe no hace nada.
     *
     * @param key nombre del usuario a buscar.
     */
    void remove(const string &key)
    {
        unsigned int index = hashing_method(key);
        auto &bucket = table[index];
        int bucket_size = bucket.size();

        for (int i = 0; i < bucket_size; i++)
        {
            if (bucket.at(i)->userName == key)
            {
                // Esto es iniciar el iterador y moverlo hasta el indice correspondiente
                bucket.erase(bucket.begin() + i);
                size--;
                return;
            }
        }
    }

private:
    /**
     * @brief remueve un usuario en la tabla hash por su UserName, si este no existe no hace nada.
     *
     * @param key nombre del usuario a buscar.
     *
     * @return
     */
    unsigned int hashing_method(const string &key)
    {
        return linear_probing(key, max_size, 0);
    }
};

#endif