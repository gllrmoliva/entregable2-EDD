#ifndef HASH_TABLES
#define HASH_TABLES

#include <vector>
#include <string>
#include <iostream>

#include "functions.h"
#include "hash_functions.h"
#include <unordered_set>

using namespace std;

// Máximo de intentos de una operación en una hash table.
const int MAX_ATTEMPTS = 5000;
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
    int max_size; ///< Tamaño de la tabla hash.
    int size = 0;
    int totalCollisions = 0;                             ///< Contador global de colisiones                                            ///< Tamaño de la tabla hash.
    int (*hashing_method)(unsigned long long, int, int); ///< Puntero a la función de hash.
    vector<User *> table;                                ///< Vector que almacena punteros a onjetos User.

    /**
     * @brief Constructor para inicializar la tabla hash con un tamaño dado y un método de hash.
     * @param size Tamaño de la tabla hash.
     * @param hashing_method Puntero a la función de hash que se usará.
     */
    CloseHashTableUserId(int size, int (*hashing_method)(unsigned long long, int, int))
        : max_size(size), hashing_method(hashing_method), table(size, nullptr) {}

    /**
     * @brief Inserta un usuario en la tabla hash.
     * @param userId El ID del usuario a insertar.
     * @param user Puntero al objeto User que se va a insertar.
     */
    void insert(unsigned long long key, User *user_data)
    {
        int i = 0;
        unsigned int index = hashing_method(key, max_size, i);
        while (i < MAX_ATTEMPTS)
        {
            if (!table[index] || table[index]->userName == "DELETED_VAR")
            {
                table[index] = new User(*user_data);
                size++;
                return;
            }
            i++;
            totalCollisions++;
            index = hashing_method(key, max_size, i);
        }
        cout << "Tabla hash está llena o se alcanzó el máximo de intentos." << endl;
    }

    /**
     *@brief Devuelve el numero total de colisiones que hubo en una Tabla Hash dependiendo
     * del metodo de resolucion de colisiones utilizado
     *
     * @return Numero de colisiones totales
     */
    int getCollision()
    {
        return totalCollisions;
    }

    /**
     * @brief Busca un usuario en la tabla hash por su ID.
     * @param userId El ID del usuario a buscar.
     * @return Puntero al objeto User si se encuentra, nullptr en caso contrario.
     */
    User *search(unsigned long long userId)
    {
        int i = 0;
        int index;
        do
        {
            index = hashing_method(userId, max_size, i);
            if (table[index] == nullptr)
                return nullptr;
            if (table[index]->userId == userId)
                return table[index];
            i++;
        } while (i < max_size);
        return nullptr;
    }

    /**
     * @brief remueve un usuario en la tabla hash por su UserId, si este no existe no hace nada.
     *
     * @param key nombre del usuario a buscar.
     */
    void remove(unsigned long long &key)
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
            if (table[index]->userId == key)
            {
                table[index] = new User(DELETED_VAR);
                size--;
                return;
            }
            i++;
            index = hashing_method(key, max_size, i);
        }
    }

    /**
     * @brief Devuelve la cantidad de espacio usado por la estructura de datos en bytes.
     */
    size_t get_memory_usage()
    {
        size_t count = 0;
        // considerando el tamaño promedio de un usuario en memoria de 70 bytes
        int user_size = 70;

        for (auto element : table)
        {
            count += 8; //< tamaño de los punteros
            if (element)
            {
                count += user_size;
            }
        }
        // espacio usado por el resto de variables
        count += sizeof(max_size);
        count += sizeof(size);

        return count;
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
    int totalCollisions = 0;
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
    void insert(unsigned long long userId, User *user)
    {

        unsigned int index = hashing_method(userId);
        if (!table[index].empty())
        {
            totalCollisions++;
        }
        table[index].push_back(user);
        size++;
    }

    /**
     *@brief Devuelve el numero total de colisiones que hubo en una Tabla Hash dependiendo
     * del metodo de resolucion de colisiones utilizado
     *
     * @return Numero de colisiones totales
     */
    int getCollision()
    {
        return totalCollisions;
    }

    /**
     * @brief Busca un usuario en la tabla hash por su ID.
     *
     * @param userId El ID del usuario a buscar.
     * @return Un puntero al objeto User encontrado, o nullptr si no se encontró.
     */
    User *search(unsigned long long userId)
    {
        unsigned int index = hashing_method(userId);
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
    void remove(unsigned long long key)
    {
        unsigned int index = hashing_method(key);
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

    /**
     * @brief Devuelve la cantidad de espacio usado por la estructura de datos en bytes.
     */
    size_t get_memory_usage()
    {
        size_t count = 0;
        // considerando el tamaño promedio de un usuario en memoria de 70 bytes
        int user_size = 70;

        for (auto bucket : table)
        {
            // tamaño usado por vector
            count += sizeof(bucket);

            for (auto element : bucket)
            {
                count += 8 + user_size; //< Se le esta sumando el tamaño del puntero (lo que se guarda) y del struct
            }
        }
        // espacio usado por el resto de variables
        count += sizeof(max_size);
        count += sizeof(size);

        return count;
    }

private:
    /**
     * @brief remueve un usuario en la tabla hash por su UserID, si este no existe no hace nada.
     *
     * @param key ID del usuario a buscar.
     *
     * @return
     */
    unsigned int hashing_method(unsigned long long key)
    {
        return key % max_size;
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
    int totalCollisions = 0;
    unsigned int (*hashing_method)(const string &, int, int);
    vector<User *> table;

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

    ~CloseHashTableUserName()
    {
        for (User *user : table)
        {
            delete user;
        }
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
            if (!table[index] || table[index]->userName == "DELETED_VAR")
            {
                table[index] = new User(*user_data);
                size++;
                return;
            }
            i++;
            totalCollisions++;
            index = hashing_method(key, max_size, i);
        }
        cout << "Tabla hash está llena o se alcanzó el máximo de intentos." << endl;
    }

    /**
     *@brief Devuelve el numero total de colisiones que hubo en una Tabla Hash dependiendo
     * del metodo de resolucion de colisiones utilizado
     *
     * @return Numero de colisiones totales
     */
    int getCollision()
    {
        return totalCollisions;
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
                return table[index];
            }
            i++;
            index = hashing_method(key, max_size, i);
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

    /**
     * @brief Devuelve la cantidad de espacio usado por la estructura de datos en bytes.
     */
    size_t get_memory_usage()
    {
        size_t count = 0;
        // considerando el tamaño promedio de un usuario en memoria de 70 bytes
        int user_size = 70;

        for (auto element : table)
        {
            count += 8; //< tamaño de los punteros
            if (element)
            {
                count += user_size;
            }
        }
        // espacio usado por el resto de variables
        count += sizeof(max_size);
        count += sizeof(size);

        return count;
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
    int totalCollisions = 0;
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
        if (!table[index].empty())
        {
            totalCollisions++;
        }

        table[index].push_back(user_data);
        size++;
    }

    /**
     *@brief Devuelve el numero total de colisiones que hubo en una Tabla Hash dependiendo
     * del metodo de resolucion de colisiones utilizado
     *
     * @return Numero de colisiones totales
     */
    int getCollision()
    {
        return totalCollisions;
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
                return user;
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

    /**
     * @brief Devuelve la cantidad de espacio usado por la estructura de datos en bytes.
     */
    size_t get_memory_usage()
    {
        size_t count = 0;
        // considerando el tamaño promedio de un usuario en memoria de 70 bytes
        int user_size = 70;

        for (auto bucket : table)
        {
            // tamaño usado por vector
            count += sizeof(bucket);

            for (auto element : bucket)
            {
                count += 8 + user_size; //< Se le esta sumando el tamaño del puntero (lo que se guarda) y del struct
            }
        }
        // espacio usado por el resto de variables
        count += sizeof(max_size);
        count += sizeof(size);

        return count;
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
        return hash_string(key) % max_size;
    }
};

#endif