#include <math.h>
#include <numeric>
#include <unordered_map>
#include <vector>
#include <string>
#include <cstdint>
#include <chrono>

#include "hash_functions.h"
#include "hash_tables.h"
#include "functions.h"
#include "time_tests.h"

using namespace std;

void test_windows()
{
  vector<User> users = readCSV("D:/Joako/Desktop/Archivos de la U/Estructura de datos/entregable2-EDD/universities_followers.csv");

  if (users.empty())
  {
    cout << "No se leyeron usuarios del archivo CSV." << endl;
  }

  HashTable ht_linear(30103, linear_probing);
  HashTable ht_quadratic(30103, quadratic_probing);
  HashTable ht_double(30103, double_hashing);
  HashTableChaining ht_chaining(30103);
  unordered_map<uint64_t, User *> ht_unordered_map;

  for (const auto &user : users)
  {
    ht_linear.insert(new User(user.university, user.userId, user.userName, user.numberTweets, user.friendsCount, user.followersCount, user.createdAt));
    ht_quadratic.insert(new User(user.university, user.userId, user.userName, user.numberTweets, user.friendsCount, user.followersCount, user.createdAt));
    ht_double.insert(new User(user.university, user.userId, user.userName, user.numberTweets, user.friendsCount, user.followersCount, user.createdAt));
    ht_chaining.insert(new User(user.university, user.userId, user.userName, user.numberTweets, user.friendsCount, user.followersCount, user.createdAt));
    ht_unordered_map[user.userId] = new User(user.university, user.userId, user.userName, user.numberTweets, user.friendsCount, user.followersCount, user.createdAt);
  }

  uint64_t userIdToSearch;
  cout << "Ingrese un userId para buscar: ";
  cin >> userIdToSearch;

  auto foundUserLinear = ht_linear.search(userIdToSearch);
  cout << "Busqueda con Linear Probing:" << endl;
  printUser(foundUserLinear);

  auto foundUserQuadratic = ht_quadratic.search(userIdToSearch);
  cout << "Busqueda con Quadratic Probing:" << endl;
  printUser(foundUserQuadratic);

  auto foundUserDouble = ht_double.search(userIdToSearch);
  cout << "Busqueda con Double Hashing:" << endl;
  printUser(foundUserDouble);

  auto foundUserChaining = ht_chaining.search(userIdToSearch);
  cout << "Busqueda con Encadenamiento:" << endl;
  printUser(foundUserChaining);

  auto foundUserUnorderedMap = ht_unordered_map.find(userIdToSearch) != ht_unordered_map.end() ? ht_unordered_map[userIdToSearch] : nullptr;
  cout << "Busqueda con unordered_map:" << endl;
  printUser(foundUserUnorderedMap);
}

void test_linux()
{
  // unordered_map<string, User> unordered_hashmap;

  vector<User> users = readCSV("universities_followers.csv");

  const int N = 42157;
  // OpenHashTableUserName ht_test(N);

  // auto start = chrono::high_resolution_clock::now();

  // for (auto &user : users)
  // {
  //   unordered_hashmap[user.userName] = user;
  // }

  // for (auto it = unordered_hashmap.begin(); it != unordered_hashmap.end(); /* no incrementamos aquí */)
  // {

  //   cout << "Eliminando usuario: " << it->second.userName << endl;
  //   it = unordered_hashmap.erase(it); // erase devuelve el siguiente iterador válido
  // }
  // auto end = chrono::high_resolution_clock::now();
  // chrono::duration<double> duration = end - start;
  // cout << "Tiempo: " << duration.count() << " segundos" << endl;

  CloseHashTableUserName hola(N, linear_probing_usename);

  return;
}

int main(int argc, char const *argv[])
{
  test_linux();
  return 0;
}