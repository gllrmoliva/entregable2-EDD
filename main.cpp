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
  CloseHashTableUserId ht_linear(30103, linear_probing);
  CloseHashTableUserId ht_quadratic(30103, quadratic_probing);
  CloseHashTableUserId ht_double(30103, double_hashing);
  OpenHashTableUserId ht_chaining(30103);
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

  size_t unordered_map_size = total_elements * (sizeof(uint64_t) + sizeof(User *));

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
}

void test_linux()
{
  vector<User> users = readCSV("universities_followers.csv");
  const int table_size = 22079;

  // Pruebas con clase User ID
  test_insert(user_id_open, table_size, users, 20000);
  test_insert(unordered_map_by_id, table_size, users, 20000);
  test_insert(user_id_close, table_size, users, 20000, linear_probing, nullptr);
  test_insert(user_id_close, table_size, users, 20000, linear_probing, nullptr);
  test_insert(user_id_close, table_size, users, 20000, linear_probing, nullptr);
  test_inserts_by_username(10, users, table_size, "test");
}

int main(int argc, char const *argv[])
{
  test_linux();
  return 0;
}
