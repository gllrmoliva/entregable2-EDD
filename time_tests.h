#ifndef TIME_TESTS
#define TIME_TESTS

#include <fstream>
#include <chrono>
#include "hash_functions.h"
#include "hash_tables.h"
#include "functions.h"
#include <numeric>

void test_insert_by_username(vector<User> &users, CloseHashTableUserName &HashTable, int N)
{
    auto start = std::chrono::high_resolution_clock::now();
    for (User user : users)
    {
        HashTable.insert(user.userName, user);
    }
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> duration = end - start;

    std::cout << "Tiempo de ejecución: " << duration.count() << " segundos." << std::endl;
}

void test_insert_by_username(vector<User> &users, OpenHashTableUserName &HashTable, int N)
{
    auto start = std::chrono::high_resolution_clock::now();
    for (User user : users)
    {
        HashTable.insert(user.userName, user);
    }
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> duration = end - start;

    std::cout << "Tiempo de ejecución: " << duration.count() << " segundos." << std::endl;
}

using namespace std;

#endif