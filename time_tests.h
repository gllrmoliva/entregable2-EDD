#ifndef TIME_TESTS
#define TIME_TESTS

#include <fstream>
#include <chrono>
#include "hash_functions.h"
#include "hash_tables.h"
#include "functions.h"
#include <numeric>

using namespace std;
using namespace std::chrono;

void insertarYCalcularTiempo(const vector<User>& users, int numInsertions, int tableSize, int (*hashing_method)(uint64_t, int, int), const string& hashTableType)
{
    // Crear una nueva tabla hash para cada medición
    HashTable ht(tableSize, hashing_method);
    
    auto start = chrono::high_resolution_clock::now(); // Marcar el tiempo de inicio
    
    // Insertar los primeros 'numInsertions' usuarios en la tabla hash recién creada
    for (int i = 0; i < numInsertions; ++i)
    {
        const auto& user = users[i];
        ht.insert(user.userId, new User(user.university, user.userId, user.userName, user.numberTweets, user.friendsCount, user.followersCount, user.createdAt));
    }

    auto end = chrono::high_resolution_clock::now(); // Marcar el tiempo de fin

    // Calcular el tiempo transcurrido en milisegundos
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    
    // Imprimir el tiempo transcurrido junto con el tipo de tabla hash
    cout << "Tiempo transcurrido despues de " << numInsertions << " inserciones en " << hashTableType << ": " << duration.count() << " ms" << endl;
}

void insertarYCalcularTiempo2(const vector<User>& users, int numInsertions, int tableSize, const string& hashTableType)
{
    // Crear una nueva tabla hash para cada medición
    HashTableChaining ht_chaining(tableSize);
    
    auto start = chrono::high_resolution_clock::now(); // Marcar el tiempo de inicio
    
    // Insertar los primeros 'numInsertions' usuarios en la tabla hash recién creada
    for (int i = 0; i < numInsertions; ++i)
    {
        const auto& user = users[i];
        ht_chaining.insert(user.userId, new User(user.university, user.userId, user.userName, user.numberTweets, user.friendsCount, user.followersCount, user.createdAt));
    }

    auto end = chrono::high_resolution_clock::now(); // Marcar el tiempo de fin

    // Calcular el tiempo transcurrido en milisegundos
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    
    // Imprimir el tiempo transcurrido junto con el tipo de tabla hash
    cout << "Tiempo transcurrido despues de " << numInsertions << " inserciones en " << hashTableType << ": " << duration.count() << " ms" << endl;
}

void insertarYCalcularTiempo3(const vector<User>& users, int numInsertions, unordered_map<uint64_t, User*>& ht_unordered_map, const string& hashTableType)
{
    auto start = chrono::high_resolution_clock::now(); // Marcar el tiempo de inicio
    for (int i = 0; i < numInsertions; ++i)
    {
        const auto& user = users[i];
        ht_unordered_map[user.userId] = new User(user.university, user.userId, user.userName, user.numberTweets, user.friendsCount, user.followersCount, user.createdAt);
    }
    auto end = chrono::high_resolution_clock::now(); // Marcar el tiempo de fin
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "Tiempo transcurrido despues de " << numInsertions << " inserciones en " << hashTableType << ": " << duration.count() << " ms" << endl;
}

// Función para realizar búsquedas de usuarios almacenados
void tiempoPromedioUserGuardado(const std::vector<User>& users, const std::unordered_map<uint64_t, User>& hashMap) {
    auto start = std::chrono::high_resolution_clock::now();
    for (const auto& user : users) {
        auto it = hashMap.find(user.userId);
        if (it != hashMap.end()) {
            // El usuario está almacenado
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Tiempo promedio de busqueda para usuarios almacenados: " << duration.count() / users.size() << " segundos" << std::endl;
}

// Función para realizar búsquedas de usuarios no almacenados
void tiempoPromedioUserNoGuardado(const std::vector<User>& users, const std::unordered_map<uint64_t, User>& hashMap) {
    auto start = std::chrono::high_resolution_clock::now();
    for (const auto& user : users) {
        auto it = hashMap.find(user.userId + 1); // Usuario no almacenado
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Tiempo promedio de busqueda para usuarios no almacenados: " << duration.count() / users.size() << " segundos" << std::endl;
}

#endif