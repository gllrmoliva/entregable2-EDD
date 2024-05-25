#include <chrono>
#include <fstream>
#include <iostream>
#include <math.h>
#include <numeric>
#include <random>
#include <unordered_map>
#include <sstream>
#include <vector>
#include <string>
#include <cstdint>

using namespace std;

const int MAX_ATTEMPTS = 100;
/*
Almacena los datos de un usuario de X, estos datos son:
- university
- userId
- userName
- numberTweets
- friendsCount
- followersCount
- createdAt
*/
struct User
{
  string university;
  uint64_t userId; // este tipo de dato es un entero positivo muy grande (64 bits)
  string userName;
  int numberTweets;
  int friendsCount;
  int followersCount;
  string createdAt;

  // Este es el constructor de User, simplemente asigna las valor a las variables
  User(const string &uni, uint64_t id, const string &name, int tweets, int friends, int followers, const string &created)
      : university(uni), userId(id), userName(name), numberTweets(tweets), friendsCount(friends), followersCount(followers), createdAt(created) {}
};

/*
Carga los datos del CSV y los pasa a un vector de la STL
información para crear el codigo:
https://www.geeksforgeeks.org/how-to-read-data-from-csv-file-to-a-2d-array-in-cpp/
*/
vector<User> readCSV(const std::string &filename)
{
  vector<User> users;
  ifstream file(filename);
  string line;

  // Salta la primera línea del csv ("titulos", nose como se llaman xd)
  getline(file, line);

  while (getline(file, line))
  {
    stringstream ss(line);
    string item;
    vector<string> tokens;

    // separamos el string en partes con respecto a la ','
    while (getline(ss, item, ','))
    {
      tokens.push_back(item);
    }

    // aseguramos que nos este dando los 7 datos, si es así entonces los agregamos al vector
    if (tokens.size() == 7)
    {
      string university = tokens[0];
      uint64_t userId = stoull(tokens[1]);
      string userName = tokens[2];
      int numberTweets = stoi(tokens[3]);
      int friendsCount = stoi(tokens[4]);
      int followersCount = stoi(tokens[5]);
      string createdAt = tokens[6];

      // emplace_back esta buena, no necesitas crear por fuera el objeto, lo crea solo, pero obviamente
      // le tienes que dar los parametros
      users.emplace_back(university, userId, userName, numberTweets, friendsCount, followersCount, createdAt);
    }
  }

  cout << "Leidos " << users.size() << " usuarios del archivo CSV." << endl;

  return users;
}

//--- Funciones hash ---

// Método de la división
// k: clave a la cual aplicaremos la función hash
// n: tamaño de la tabla hash
int h1(uint64_t k, int n) { return k % n; }

// Método de la multiplicación
// k: clave a la cual aplicaremos la función hash
// n: tamaño de la tabla hash
int h2(uint64_t k, int n)
{
  return k * 53 + k * k * 13 + 18;
}

//--- Métodos de Open addressing o hashing cerrado ---

// Linear probing
// k: clave a la cual aplicaremos la función hash
// n: tamaño de la tabla hash
// i: número del intento
int linear_probing(uint64_t k, int n, int i)
{
  // Utilizando el método de la division
  return (h1(k, n) + i) % n;
}

// Quadratic probing
// k: clave a la cual aplicaremos la función hash
// n: tamaño de la tabla hash
// i: número del intento
int quadratic_probing(uint64_t k, int n, int i)
{
  // Utilizando el método de la division
  return (h1(k, n) + i + 2 * i * i) % n;
}

// Double hashing
// k: clave a la cual aplicaremos la función hash
// n: tamaño de la tabla hash
// i: número del intento
int double_hashing(uint64_t k, int n, int i)
{
  // Utilizando como primer método el método de la division y luego el
  // método de la multiplicacion
  return (h1(k, n) + i * (h2(k, n) + 1)) % n;
}

class HashTable
{
public:
  int size;
  vector<User *> table;
  int (*hashing_method)(uint64_t, int, int);

  HashTable(int size, int (*hashing_method)(uint64_t, int, int))
      : size(size), hashing_method(hashing_method), table(size, nullptr) {}

  void insert(User *user)
  {
    int i = 0;
    int index;
    do
    {
      index = hashing_method(user->userId, size, i);
      if (table[index] == nullptr)
      {
        table[index] = user;
        return;
      }
      i++;
    } while (i < size);
    cout << "Error: Hash table overflow" << endl;
  }

  User *search(uint64_t userId)
  {
    int i = 0;
    int index;
    do
    {
      index = hashing_method(userId, size, i);
      if (table[index] == nullptr)
        return nullptr;
      if (table[index]->userId == userId)
        return table[index];
      i++;
    } while (i < size);
    return nullptr;
  }
  ~HashTable()
  {
    for (auto user : table)
    {
      delete user;
    }
  }
};
//-------------FUNCIONES DE HASHEO PARA KEY USERNAME----------------//
int hash_string(const string &str)
{
  // en esta es la funcion vista en clases, en este caso se utilizo el numero
  //  31 porque es primo y lo recomendaban en diversas fuentes
  int p = 31;
  int hash_value = 0;
  int p_pow = 1;
  for (char c : str)
  {
    hash_value += (c - 'a' + 1) * p_pow;
    p_pow *= p;
  }
  return abs(hash_value);
}

int linear_probing_usename(const string &userName, int n, int i)
{
  int k = hash_string(userName);

  // Utilizando el método de la division
  return (h1(k, n) + i) % n;
}
int quadratic_probing_username(const string &userName, int n, int i)
{
  int k = hash_string(userName);

  // Utilizando el método de la division
  return (h1(k, n) + i + 2 * i * i) % n;
}

int double_hashing_username(const string &userName, int n, int i)
{
  int k = hash_string(userName);
  // Utilizando como primer método el método de la division y luego el
  // método de la multiplicacion
  return abs((h1(k, n) + i * (h2(k, n) + 1)) % n);
}

//------ HASH TABLE USER NAME--------//
// TODO: discutir si reutilizar las funciones hash ya propuestas o si crear unas unicas
// para el caso de
class HashTableUserName
{
public:
  int size;
  User **table; // Notemos que esto es una tabla con punteros de struct User
  int (*hashing_method)(const string &, int, int);

  HashTableUserName(int size, int (*hashing_method)(const string &, int, int))
      : size(size), hashing_method(hashing_method)
  {
    table = new User *[size];
    for (int i = 0; i < size; i++)
    {
      table[i] = nullptr;
    }
  }

  ~HashTableUserName()
  {
    delete[] table;
  }

  void insert(const string &key, User *user_data)
  {
    int i = 0;
    unsigned int index = hashing_method(key, size, i);
    while (table[index] != nullptr && (table[index]->userName != "DELETED_VAR") && i <= MAX_ATTEMPTS)
    {
      i++;
      index = hashing_method(key, size, i);
    }
    if (i <= MAX_ATTEMPTS)
      table[index] = user_data;
  }

  User *search(const string &key)
  {
    int i = 0;
    unsigned int index = hashing_method(key, size, i);
    while (table[index] != nullptr && table[index]->userName != key && i <= MAX_ATTEMPTS)
    {
      i++;
      index = hashing_method(key, size, i);
    }
    if (table[index] != nullptr && table[index]->userName == key && i <= MAX_ATTEMPTS)
    {
      cout << "Encontrado: " << endl;
      cout << table[index]->userName << endl;
      cout << table[index]->userId << endl;
      cout << table[index]->university << endl;
      cout << "indice en tabla hash: " << index << endl;

      return table[index];
    }
    cout << "No se encontro el usuario" << endl;
    return nullptr; // No se encontró el usuario
  }

  void remove(const string &key)
  {
    int i = 0;
    unsigned int index = hashing_method(key, size, i);
    while (table[index] != nullptr && table[index]->userName != key)
    {
      i++;
      index = hashing_method(key, size, i);
    }
    if (table[index] != nullptr && table[index]->userName == key)
    {
      // Se usa un usuario con el nombre DELETED_VAR
      User *DELETED_VAR = new User("", 0, "DELETED_VAR", 0, 0, 0, "");
      table[index] = DELETED_VAR;
    }
  }
};

//---------------FUNCIONES TEST--------------------//

// Tablaa hash con separate chaining (encadenamiento)
class HashTableChaining
{
public:
  int size;
  vector<vector<User *>> table;

  HashTableChaining(int size) : size(size), table(size) {}

  void insert(User *user)
  {
    int index = user->userId % size;
    table[index].push_back(user);
  }

  User *search(uint64_t userId)
  {
    int index = userId % size;
    for (User *user : table[index])
    {
      if (user->userId == userId)
        return user;
    }
    return nullptr;
  }

  ~HashTableChaining()
  {
    for (auto &bucket : table)
    {
      for (auto user : bucket)
      {
        delete user;
      }
    }
  }
};

void printUser(User *foundUser)
{
  if (foundUser)
  {
    cout << "Usuario encontrado: " << foundUser->userName << endl;
    cout << "Universidad: " << foundUser->university << endl;
    cout << "Numero de Tweets: " << foundUser->numberTweets << endl;
    cout << "Numero de amigos: " << foundUser->friendsCount << endl;
    cout << "Numero de seguidores: " << foundUser->followersCount << endl;
    cout << "Fecha de creacion: " << foundUser->createdAt << endl;
  }
  else
  {
    cout << "Usuario no encontrado" << endl;
  }
}

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
  vector<User> users = readCSV("test_data_copied.csv");

  const int N = 42157;
  HashTableUserName ht_test(N, double_hashing_username);

  for (auto &user : users)
  {
    ht_test.insert(user.userName, &user);
  }

  ht_test.search("SantillanaLAB");
  ht_test.remove("SantillanaLAB");
  ht_test.search("SantillanaLAB");

  ht_test.remove("SantillanaLAB");
  ht_test.search("SantillanaLAB");

  ht_test.remove("SantillanaLAB");
  ht_test.search("SantillanaLAB");

  ht_test.remove("SantillanaLAB");
  ht_test.search("SantillanaLAB");

  ht_test.insert(users[0].userName, &users[0]);
  ht_test.search("SantillanaLAB");
}

int main(int argc, char const *argv[])
{
  test_linux();
  return 0;
}