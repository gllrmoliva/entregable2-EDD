#ifndef TIME_TESTS
#define TIME_TESTS

#include <fstream>
#include <chrono>
#include <numeric>
#include <unordered_map>

#include "hash_functions.h"
#include "hash_tables.h"
#include "functions.h"

using namespace std;

// TODO: claramente tiene más sentido hacer un polimorfismo para que la función sea global para
// todas las tablas hash, pero por ahora sera con overloading
chrono::duration<double> test_insert_by_username(vector<User> &users, CloseHashTableUserName &HashTable, int n_tests)
{
    vector<chrono::duration<double>> times;
    chrono::duration<double> average = chrono::duration<double>(0);

    for (int i = 0; i < n_tests; i++)
    {
        auto start = chrono::high_resolution_clock::now();
        for (User user : users)
        {
            HashTable.insert(user.userName, &user);
        }
        auto end = chrono::high_resolution_clock::now();

        for (User user : users)
        {
            HashTable.remove(user.userName);
        }

        chrono::duration<double> duration = end - start;
        times.push_back(duration);

        cout << "- test " << i << ":  " << duration.count() << " segundos." << endl;
    }

    for (auto time : times)
    {
        average += time;
    }
    average = average / n_tests;

    std::cout << "Tiempo promedio: " << average.count() << " segundos." << std::endl;
    return average;
}

chrono::duration<double> test_insert_by_username(vector<User> &users, OpenHashTableUserName &HashTable, int n_tests)
{
    vector<chrono::duration<double>> times;
    chrono::duration<double> average = chrono::duration<double>(0);

    for (int i = 0; i < n_tests; i++)
    {
        auto start = chrono::high_resolution_clock::now();
        for (User user : users)
        {
            HashTable.insert(user.userName, user);
        }
        auto end = chrono::high_resolution_clock::now();

        for (User user : users)
        {
            HashTable.remove(user.userName);
        }

        chrono::duration<double> duration = end - start;
        times.push_back(duration);

        cout << "- test " << i << ":  " << duration.count() << " segundos." << endl;
    }

    for (auto time : times)
    {
        average += time;
    }
    average = average / n_tests;

    std::cout << "Tiempo promedio: " << average.count() << " segundos." << std::endl;
    return average;
}

chrono::duration<double> test_insert_by_username(vector<User> &users, unordered_map<string, User> &HashTable, int n_tests)
{
    vector<chrono::duration<double>> times;
    chrono::duration<double> average = chrono::duration<double>(0);

    for (int i = 0; i < n_tests; i++)
    {
        auto start = chrono::high_resolution_clock::now();
        for (User user : users)
        {
            HashTable[user.userName] = user;
        }
        auto end = chrono::high_resolution_clock::now();

        for (User user : users)
        {
            HashTable.erase(user.userName);
        }

        chrono::duration<double> duration = end - start;
        times.push_back(duration);

        cout << "- test " << i << ":  " << duration.count() << " segundos." << endl;
    }

    for (auto time : times)
    {
        average += time;
    }
    average = average / n_tests;

    std::cout << "Tiempo promedio: " << average.count() << " segundos." << std::endl;
    return average;
}
#endif