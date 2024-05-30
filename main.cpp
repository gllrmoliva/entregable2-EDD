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

  std::cout << "Tamaño del objeto User: " << sizeof(User) << " bytes" << std::endl;

  size_t ht_probing_size = 30103 * sizeof(User*);

  std::cout << "Tamaño de Tabla Hash con probing en KB: " << ht_probing_size / 1024.0 << " KB" << std::endl;

  size_t ht_chaining_base_size = 30103 * sizeof(std::vector<User*>);

  // Suponemos un promedio de 1.5 elementos por bucket
  double average_elements_per_bucket = 1.5;
  size_t total_elements = static_cast<size_t>(30103 * average_elements_per_bucket);
  size_t ht_chaining_total_size = ht_chaining_base_size + total_elements * sizeof(User*);

  double htChainingBaseSizeMB = static_cast<double>(ht_chaining_base_size) / (1024 * 1024);
  double htChainingTotalSizeMB = static_cast<double>(ht_chaining_total_size) / (1024 * 1024);

  // Imprimir resultados
  std::cout << "Tamaño base de HashTableChaining: " << htChainingBaseSizeMB << " MB" << std::endl;
  std::cout << "Tamaño total de HashTableChaining: " << htChainingTotalSizeMB << " MB" << std::endl;

  size_t unordered_map_size = total_elements * (sizeof(uint64_t) + sizeof(User*));

  double unorderedMapSizeKB = static_cast<double>(unordered_map_size) / 1024;

  // Imprimir el resultado
  std::cout << "Tamaño aproximado de std::unordered_map: " << unorderedMapSizeKB << " KB" << std::endl;

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

  // Crear estructuras de datos para almacenar usuarios
  std::unordered_map<uint64_t, User> hashMap;

  // Insertar usuarios en las estructuras de datos
  insertUsers(users, hashMap);

  // Medir el tiempo promedio de búsqueda para usuarios almacenados
  tiempoPromedioUserGuardado2(users, &ht_linear, "Linear Probing");
  tiempoPromedioUserGuardado2(users, &ht_quadratic, "Quadratic Probing");
  tiempoPromedioUserGuardado2(users, &ht_double, "Double Hashing");
  tiempoPromedioUserGuardado3(users, "Chaining");
  tiempoPromedioUserGuardado(users, hashMap);

  // Medir el tiempo de búsqueda para usuarios no almacenados
  tiempoPromedioUserNoGuardado2(users, &ht_linear, "Linear Probing");
  tiempoPromedioUserNoGuardado2(users, &ht_quadratic, "Quadratic Probing");
  tiempoPromedioUserNoGuardado2(users, &ht_double, "Double Hashing");
  tiempoPromedioUserNoGuardado3(users, "Chaining");
  tiempoPromedioUserNoGuardado(users, hashMap);
  
  int tableSize = 30103;

  insertarYCalcularTiempo(users, 1000, tableSize, linear_probing, "Linear Probing");
  insertarYCalcularTiempo(users, 5000, tableSize, linear_probing, "Linear Probing");
  insertarYCalcularTiempo(users, 10000, tableSize, linear_probing, "Linear Probing");
  insertarYCalcularTiempo(users, 15000, tableSize, linear_probing, "Linear Probing");
  insertarYCalcularTiempo(users, 20000, tableSize, linear_probing, "Linear Probing");

  insertarYCalcularTiempo(users, 1000, tableSize, quadratic_probing, "Quadratic Probing");
  insertarYCalcularTiempo(users, 5000, tableSize, quadratic_probing, "Quadratic Probing");
  insertarYCalcularTiempo(users, 10000, tableSize, quadratic_probing, "Quadratic Probing");
  insertarYCalcularTiempo(users, 15000, tableSize, quadratic_probing, "Quadratic Probing");
  insertarYCalcularTiempo(users, 20000, tableSize, quadratic_probing, "Quadratic Probing");

  insertarYCalcularTiempo(users, 1000, tableSize, double_hashing, "Double Hashing");
  insertarYCalcularTiempo(users, 5000, tableSize, double_hashing, "Double Hashing");
  insertarYCalcularTiempo(users, 10000, tableSize, double_hashing, "Double Hashing");
  insertarYCalcularTiempo(users, 15000, tableSize, double_hashing, "Double Hashing");
  insertarYCalcularTiempo(users, 20000, tableSize, double_hashing, "Double Hashing");

  insertarYCalcularTiempo2(users, 1000, tableSize, "Chaining");
  insertarYCalcularTiempo2(users, 5000, tableSize, "Chaining");
  insertarYCalcularTiempo2(users, 10000, tableSize, "Chaining");
  insertarYCalcularTiempo2(users, 15000, tableSize, "Chaining");
  insertarYCalcularTiempo2(users, 20000, tableSize, "Chaining");

  insertarYCalcularTiempo3(users, 1000, ht_unordered_map, "Unordered_map");
  insertarYCalcularTiempo3(users, 5000, ht_unordered_map, "Unordered_map");
  insertarYCalcularTiempo3(users, 10000, ht_unordered_map, "Unordered_map");
  insertarYCalcularTiempo3(users, 15000, ht_unordered_map, "Unordered_map");
  insertarYCalcularTiempo3(users, 20000, ht_unordered_map, "Unordered_map");
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
  test_windows();
  return 0;
}
