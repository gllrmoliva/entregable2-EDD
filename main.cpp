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

#include "hash_functions.h"
#include "hash_tables.h"
#include "functions.h"

using namespace std;

/**
 * @brief Prueba varias implementaciones de tablas hash para buscar usuarios a partir de un archivo CSV.
 * 
 * Esta función lee los datos de usuarios desde un archivo CSV y los inserta en varias tablas hash
 * utilizando diferentes métodos de resolución de colisiones (Linear Probing, Quadratic Probing, Double Hashing y Encadenamiento).
 * Luego, permite al usuario buscar un usuario por su ID, ingresado en notación científica o decimal, y muestra los resultados de la búsqueda.
 */
void test_windows()
{

  // Leer usuarios desde el archivo CSV
  vector<User> users = readCSV("D:/Joako/Desktop/Archivos de la U/Estructura de datos/entregable2-EDD/universities_followers.csv");

  if (users.empty())
  {
    std::cout << "No se leyeron usuarios del archivo CSV." << endl;
  }

  // Crear tablas hash con diferentes métodos de resolución de colisiones
  HashTable ht_linear(30103, linear_probing);
  HashTable ht_quadratic(30103, quadratic_probing);
  HashTable ht_double(30103, double_hashing);
  HashTableChaining ht_chaining(30103);
  unordered_map<uint64_t, User *> ht_unordered_map;

  // Insertar usuarios en cada tabla hash
  for (const auto &user : users)
  {
    ht_linear.insert(user.userId, new User(user.university, user.userId, user.userName, user.numberTweets, user.friendsCount, user.followersCount, user.createdAt));
    ht_quadratic.insert(user.userId, new User(user.university, user.userId, user.userName, user.numberTweets, user.friendsCount, user.followersCount, user.createdAt));
    ht_double.insert(user.userId, new User(user.university, user.userId, user.userName, user.numberTweets, user.friendsCount, user.followersCount, user.createdAt));
    ht_chaining.insert(user.userId, new User(user.university, user.userId, user.userName, user.numberTweets, user.friendsCount, user.followersCount, user.createdAt));
    ht_unordered_map[user.userId] = new User(user.university, user.userId, user.userName, user.numberTweets, user.friendsCount, user.followersCount, user.createdAt);
  }

  // Pedir al usuario que ingrese un ID de usuario a buscar
  std::string userIdInput;
  cout << "Ingrese un userId para buscar: ";
  cin >> userIdInput;

  // Convertir el ID de usuario ingresado de notación científica a decimal si es necesario
  uint64_t userIdToSearch = scientificToNormal(userIdInput);

  cout << "Buscando usuario con userId: " << userIdToSearch << endl;

  // Buscar el usuario en cada tabla hash y mostrar los resultados
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
  vector<User> users = readCSV("universities_followers.csv");
  const int table_size = 22079;
  int n_tests = 1;

  CloseHashTableUserName linear_table(table_size, linear_probing);
  CloseHashTableUserName double_table(table_size, double_hashing);
  CloseHashTableUserName quadratic_table(table_size, quadratic_probing);
  OpenHashTableUserName open_table(table_size);
  unordered_map<string, User> unordered_map;

  cout << "LINEAR" << endl;
  test_insert_by_username(users, linear_table, n_tests);
  cout << endl;
  cout << "DOUBLE" << endl;
  test_insert_by_username(users, double_table, n_tests);
  cout << endl;
  cout << "QUADRATIC" << endl;
  test_insert_by_username(users, quadratic_table, n_tests);
  cout << endl;
  cout << "OPEN" << endl;
  test_insert_by_username(users, open_table, n_tests);
  cout << "UNORDERED MAP" << endl;
  test_insert_by_username(users, unordered_map, n_tests);
}

int main(int argc, char const *argv[])
{
  test_linux();
  return 0;
}
