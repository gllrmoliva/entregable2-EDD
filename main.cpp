#include <math.h>
#include <numeric>
#include <unordered_map>
#include <vector>
#include <string>
#include <chrono>
#include <bits/stdc++.h>

#include "hash_functions.h"
#include "hash_tables.h"
#include "functions.h"
#include "time_tests.h"

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
  CloseHashTableUserId ht_linear(30103, linear_probing);
  CloseHashTableUserName ht_quadratic2(30103, quadratic_probing);
  CloseHashTableUserId ht_double(30103, double_hashing);
  OpenHashTableUserName ht_chaining2(30103);
  OpenHashTableUserId ht_chaining(30103);
  CloseHashTableUserId ht_quadratic(30103, quadratic_probing);
  unordered_map<unsigned long long, User *> ht_unordered_map;

  // Insertar usuarios en cada tabla hash
  for (const auto &user : users)
  {
    ht_linear.insert(user.userId, new User(user.university, user.userId, user.userName, user.numberTweets, user.friendsCount, user.followersCount, user.createdAt));
    ht_quadratic2.insert(user.userName, new User(user.university, user.userId, user.userName, user.numberTweets, user.friendsCount, user.followersCount, user.createdAt));
    ht_double.insert(user.userId, new User(user.university, user.userId, user.userName, user.numberTweets, user.friendsCount, user.followersCount, user.createdAt));
    ht_chaining2.insert(user.userName, new User(user.university, user.userId, user.userName, user.numberTweets, user.friendsCount, user.followersCount, user.createdAt));
    ht_unordered_map[user.userId] = new User(user.university, user.userId, user.userName, user.numberTweets, user.friendsCount, user.followersCount, user.createdAt);
    ht_quadratic.insert(user.userId, new User(user.university, user.userId, user.userName, user.numberTweets, user.friendsCount, user.followersCount, user.createdAt));
    ht_chaining.insert(user.userId, new User(user.university, user.userId, user.userName, user.numberTweets, user.friendsCount, user.followersCount, user.createdAt));
  }

  std::cout << "Tamaño del objeto User: " << sizeof(User) << " bytes" << std::endl;

  size_t ht_probing_size = 30103 * sizeof(User *);

  std::cout << "Tamaño de Tabla Hash con probing en KB: " << ht_probing_size / 1024.0 << " KB" << std::endl;

  size_t ht_chaining_base_size = 30103 * sizeof(std::vector<User *>);

  // Suponemos un promedio de 1.5 elementos por bucket
  double average_elements_per_bucket = 1.5;
  size_t total_elements = static_cast<size_t>(30103 * average_elements_per_bucket);
  size_t ht_chaining_total_size = ht_chaining_base_size + total_elements * sizeof(User *);

  double htChainingBaseSizeMB = static_cast<double>(ht_chaining_base_size) / (1024 * 1024);
  double htChainingTotalSizeMB = static_cast<double>(ht_chaining_total_size) / (1024 * 1024);

  // Imprimir resultados
  std::cout << "Tamaño base de HashTableChaining: " << htChainingBaseSizeMB << " MB" << std::endl;
  std::cout << "Tamaño total de HashTableChaining: " << htChainingTotalSizeMB << " MB" << std::endl;

  size_t unordered_map_size = total_elements * (sizeof(unsigned long long) + sizeof(User *));

  double unorderedMapSizeKB = static_cast<double>(unordered_map_size) / 1024;

  // Imprimir el resultado
  std::cout << "Tamaño aproximado de std::unordered_map: " << unorderedMapSizeKB << " KB" << std::endl;

  // Pedir al usuario que ingrese un ID de usuario a buscar
  std::string userIdInput;
  cout << "Ingrese un userId para buscar: ";
  cin >> userIdInput;

  // Convertir el ID de usuario ingresado de notación científica a decimal si es necesario
  unsigned long long userIdToSearch = scientificToNormal(userIdInput);

  cout << "Buscando usuario con userId: " << userIdToSearch << endl;

  // Buscar el usuario en cada tabla hash y mostrar los resultados
  auto foundUserLinear = ht_linear.search(userIdToSearch);
  cout << "Busqueda con Linear Probing:" << endl;
  printUser(foundUserLinear);

  // auto foundUserQuadratic = ht_quadratic.search(userIdToSearch);
  // cout << "Busqueda con Quadratic Probing:" << endl;
  // printUser(foundUserQuadratic);

  auto foundUserDouble = ht_double.search(userIdToSearch);
  cout << "Busqueda con Double Hashing:" << endl;
  printUser(foundUserDouble);

  // auto foundUserChaining = ht_chaining.search(userIdToSearch);
  // cout << "Busqueda con Encadenamiento:" << endl;
  // printUser(foundUserChaining);

  auto foundUserUnorderedMap = ht_unordered_map.find(userIdToSearch) != ht_unordered_map.end() ? ht_unordered_map[userIdToSearch] : nullptr;
  cout << "Busqueda con unordered_map:" << endl;
  printUser(foundUserUnorderedMap);

  std::cout << "Colisiones totales en Linear Probing: " << ht_linear.getCollision() << std::endl;
  std::cout << "Colisiones totales en Double Probing: " << ht_double.getCollision() << std::endl;
  std::cout << "Colisiones totales en Quadratic Probing: " << ht_quadratic.getCollision() << std::endl;
  std::cout << "Colisiones totales en Chaining: " << ht_chaining.getCollision() << std::endl;
  std::cout << "Colisiones totales en Quadratic2 Probing: " << ht_quadratic2.getCollision() << std::endl;
  std::cout << "Colisiones totales en Chaining2: " << ht_chaining2.getCollision() << std::endl;
}

void test_linux()
{
  vector<User> real_users = readCSV("universities_followers.csv");
  vector<User> fake_users = readCSV("fake_data.csv");
  const int table_size = 30103;
  int n_tests = 100;

  // Pruebas de inserción
  test_inserts_by_username(n_tests, real_users, table_size, "tests/insert_by_username");
  test_inserts_by_userid(n_tests, real_users, table_size, "tests/insert_by_userid");

  // Pruebas de busqueda usuarios existentes
  test_searchs_by_username(n_tests, real_users, real_users, table_size, "tests/search_by_username_realusers");
  test_searchs_by_userid(n_tests, real_users, real_users, table_size, "tests/search_by_userid_realusers");

  // Pruebas de busqueda usuarios no existentes
  test_searchs_by_username(n_tests, real_users, fake_users, table_size, "tests/search_by_username_fakeusers");
  test_searchs_by_userid(n_tests, real_users, fake_users, table_size, "tests/search_by_userid_fakeusers");

  for (int i = 0; i <= 20000; i = i + 2500)
  {
    memory_test(table_size, i, real_users, "tests/test_de_memory");
    colisions_test(table_size, i, real_users, "tests/test_colisiones");
  }
}

int main()
{
  // test_windows();
  test_linux();
  return 0;
}
