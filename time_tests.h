#ifndef TIME_TESTS
#define TIME_TESTS

#include <fstream>
#include <chrono>
#include <numeric>
#include <unordered_map>
#include <variant>

#include "hash_functions.h"
#include "hash_tables.h"
#include "functions.h"

using namespace std;
using namespace std::chrono;

enum HashTableType
{
    user_id_open,
    user_id_close,
    user_name_open,
    user_name_close,
    unordered_map_by_name,
    unordered_map_by_id,
};

// Función para realizar búsquedas de usuarios almacenados
void tiempoPromedioUserGuardado(const std::vector<User> &users, const std::unordered_map<uint64_t, User> &hashMap)
{
    auto start = std::chrono::high_resolution_clock::now();
    for (const auto &user : users)
    {
        auto it = hashMap.find(user.userId);
        if (it != hashMap.end())
        {
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Tiempo total de busqueda para usuarios almacenados en Unordered: " << duration.count() << " segundos" << std::endl;
    std::cout << "Tiempo promedio de busqueda para usuarios almacenados en Unordered: " << duration.count() / users.size() << " segundos" << std::endl;
}

// Función para realizar búsquedas de usuarios no almacenados
void tiempoPromedioUserNoGuardado(const std::vector<User> &users, const std::unordered_map<uint64_t, User> &hashMap)
{
    auto start = std::chrono::high_resolution_clock::now();
    for (const auto &user : users)
    {
        hashMap.find(user.userId + 1); // Usuario no almacenado
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Tiempo total de busqueda para usuarios no almacenados en Unordered: " << duration.count() << " segundos" << std::endl;
    std::cout << "Tiempo promedio de busqueda para usuarios no almacenados en Unordered: " << duration.count() / users.size() << " segundos" << std::endl;
}

// TODO: claramente tiene más sentido hacer un polimorfismo para que la función sea global para
// todas las tablas hash, pero por ahora sera con overloading

void tiempoPromedioUserGuardado2(const vector<User> &users, CloseHashTableUserId *ht, const string &hashTableType)
{
    auto start = std::chrono::high_resolution_clock::now();
    for (const auto &user : users)
    {
        ht->search(user.userId);
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Tiempo total de busqueda para usuarios almacenados en " << hashTableType << ": " << duration.count() << " segundos" << std::endl;
    std::cout << "Tiempo promedio de busqueda para usuarios almacenados en " << hashTableType << ": " << duration.count() / users.size() << " segundos" << std::endl;
}

void tiempoPromedioUserGuardado3(const vector<User> &users, const string &hashTableType)
{
    OpenHashTableUserId ht_chaining(30103);
    auto start = std::chrono::high_resolution_clock::now();
    for (const auto &user : users)
    {
        ht_chaining.search(user.userId);
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Tiempo total de busqueda para usuarios almacenados en " << hashTableType << ": " << duration.count() << " segundos" << std::endl;
    std::cout << "Tiempo promedio de busqueda para usuarios almacenados en " << hashTableType << ": " << duration.count() / users.size() << " segundos" << std::endl;
}

void tiempoPromedioUserNoGuardado2(const std::vector<User> &users, CloseHashTableUserId *ht, const std::string &hashTableType)
{
    auto start = std::chrono::high_resolution_clock::now();
    for (const auto &user : users)
    {
        ht->search(user.userId + 1); // Usuario no almacenado
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Tiempo total de busqueda para usuarios no almacenados en " << hashTableType << ": " << duration.count() << " segundos" << std::endl;
    std::cout << "Tiempo promedio de busqueda para usuarios no almacenados en " << hashTableType << ": " << duration.count() / users.size() << " segundos" << std::endl;
}

void tiempoPromedioUserNoGuardado3(const std::vector<User> &users, const string &hashTableType)
{
    OpenHashTableUserId ht_chaining(30103);
    auto start = std::chrono::high_resolution_clock::now();
    for (const auto &user : users)
    {
        ht_chaining.search(user.userId + 1); // Usuario no almacenado
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Tiempo total de busqueda para usuarios no almacenados en " << hashTableType << ": " << duration.count() << " segundos" << std::endl;
    std::cout << "Tiempo promedio de busqueda para usuarios no almacenados en " << hashTableType << ": " << duration.count() / users.size() << " segundos" << std::endl;
}

/**
 * @brief Dado una base de datos de usuarios en forma de vector, inserta n_inserts usuarios a una tabla hash elegida y
 * calcula el tiempo que demora hacer esta operación.
 *
 * @param type: tipo de tabla hash (del tipo key-User), los valores son parte de un enumerador.
 * @param max_size: tamaño máximo de la tabla hash.
 * @param users: Vector con datos sobre usuarios, desde aqui se sacaran los usuarios a añadir.
 * @param n_inserts: Cantidad de usuarios a insertar.
 * @param hashing_method_id: puntero a una funcion de hashing para utilizar con userId. por defecto es nullptr
 * @param hashing_method_name: puntero a una funcion de hashing para utilizar con userName. por defecto es nullptr
 *
 */
double test_insert(HashTableType type, int max_size, vector<User> &users, int n_inserts,
                   int (*hashing_method_id)(uint64_t, int, int) = nullptr,
                   unsigned int (*hashing_method_name)(const string &, int, int) = nullptr)
{
    auto start = chrono::high_resolution_clock::now();

    switch (type)
    {
    case user_id_open:
    {
        OpenHashTableUserId hash_table(max_size);
        for (int i = 0; i < n_inserts; i++)
        {
            hash_table.insert(users[i].userId, &users[i]);
        }
        break;
    }
    case user_name_open:
    {
        OpenHashTableUserName hash_table(max_size);
        for (int i = 0; i < n_inserts; i++)
        {
            hash_table.insert(users[i].userName, &users[i]);
        }
        break;
    }
    case unordered_map_by_name:
    {
        unordered_map<string, User> hash_table;
        for (int i = 0; i < n_inserts; i++)
        {
            hash_table[users[i].userName] = users[i];
        }
        break;
    }
    case unordered_map_by_id:
    {
        unordered_map<int64_t, User> hash_table;
        for (int i = 0; i < n_inserts; i++)
        {
            hash_table[users[i].userId] = users[i];
        }
        break;
    }
    case user_id_close:
    {
        if (hashing_method_id == nullptr)
        {
            cout << "el hashing_method para user_id_close no fue proporcionado." << endl;
            return 0;
        }
        CloseHashTableUserId hash_table(max_size, hashing_method_id);
        for (int i = 0; i < n_inserts; i++)
        {
            hash_table.insert(users[i].userId, &users[i]);
        }
        break;
    }
    case user_name_close:
    {
        if (hashing_method_name == nullptr)
        {
            cout << "el hashing_method para user_id_close no fue proporcionado." << endl;
            return 0;
        }
        CloseHashTableUserName hash_table(max_size, hashing_method_name);
        for (int i = 0; i < n_inserts; i++)
        {
            hash_table.insert(users[i].userName, &users[i]);
        }
        break;
    }
    default:
        cout << "Algo fallo en la función, asegurate de poner todos los parametros" << endl;
        return 0;
    }

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;

    cout << "Tiempo: " << duration.count() << " segundos." << endl;

    return duration.count();
}
#endif
