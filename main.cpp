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
  std::string university;
  uint64_t userId; // este tipo de dato es un entero positivo muy grande (64 bits)
  std::string userName;
  int numberTweets;
  int friendsCount;
  int followersCount;
  std::string createdAt;

  // Este es el constructor de User, simplemente asigna las valor a las variables
  User(const std::string &uni, uint64_t id, const std::string &name, int tweets, int friends, int followers, const std::string &created)
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
  return users;
}

/**********************/
/*** Funciones hash ***/
/**********************/

// Método de la división
// k: clave a la cual aplicaremos la función hash
// n: tamaño de la tabla hash
int h1(int k, int n) { return k % n; }

// Método de la multiplicación
// k: clave a la cual aplicaremos la función hash
// n: tamaño de la tabla hash
int h2(int k, int n)
{
  float a = (float)k * ((sqrt(5) - 1) / 2);
  a -= (int)a; // Esta línea implementa la operación módulo 1 (%1)

  return n * a;
}

/****************************************************/
/*** Métodos de Open addressing o hashing cerrado ***/
/****************************************************/

// Linear probing
// k: clave a la cual aplicaremos la función hash
// n: tamaño de la tabla hash
// i: número del intento
int linear_probing(int k, int n, int i)
{
  // Utilizando el método de la division
  return (h1(k, n) + i) % n;
}

// Quadratic probing
// k: clave a la cual aplicaremos la función hash
// n: tamaño de la tabla hash
// i: número del intento
int quadratic_probing(int k, int n, int i)
{
  // Utilizando el método de la division
  return (h1(k, n) + i + 2 * i * i) % n;
}

// Double hashing
// k: clave a la cual aplicaremos la función hash
// n: tamaño de la tabla hash
// i: número del intento
int double_hashing(int k, int n, int i)
{
  // Utilizando como primer método el método de la division y luego el
  // método de la multiplicacion
  return (h1(k, n) + i * (h2(k, n) + 1)) % n;
}

class HashTable
{
public:
  int size;
  int *table;
  int (*hashing_method)(int, int, int);

  HashTable(int size, int (*hashing_method)(int, int, int))
      : size(size), hashing_method(hashing_method)
  {
    table = new int[size];
    for (int i = 0; i < size; i++)
    {
      table[i] = -1;
    }
  }

  void insert(int key)
  {
    int i = 0;
    while (table[hashing_method(key, size, i)] != -1)
    {
      i++;
    }
    table[hashing_method(key, size, i)] = key;
  }

  bool search(int key)
  {
    int i = 0;
    while (table[hashing_method(key, size, i)] != key &&
           table[hashing_method(key, size, i)] != -1)
    {
      i++;
    }
    return table[hashing_method(key, size, i)] == key;
  }
};

void test_insert(HashTable &ht, int n_inserts, int n_tests,
                 const char *numbers_file, const char *out_file,
                 bool overwrite = false)
{
  ifstream file(numbers_file);
  ofstream file_out(out_file, overwrite ? ios::trunc : ios::app);

  vector<double> times;
  for (int i = 0; i < n_tests; i++)
  {
    int key;
    auto start = chrono::high_resolution_clock::now();
    for (int j = 0; j < n_inserts; j++)
    {
      file >> key;
      ht.insert(key);
    }
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
    times.push_back(duration.count());
  }

  // Calcular promedio tiempo
  file_out << n_inserts << ","
           << accumulate(times.begin(), times.end(), 0.0) / times.size()
           << endl;
  file.close();
  file_out.close();
}

void test_search(HashTable &ht, int n_searches, int n_tests,
                 const char *numbers_file, const char *out_file,
                 bool overwrite = false)
{
  ifstream file(numbers_file);
  ofstream file_out(out_file, overwrite ? ios::trunc : ios::app);

  vector<double> times;
  // Llenar la tabla hash
  for (int i = 0; i < n_searches; i++)
  {
    int key;
    file >> key;
    ht.insert(key);
  }

  for (int i = 0; i < n_tests; i++)
  {
    int key;
    auto start = chrono::high_resolution_clock::now();
    for (int j = 0; j < n_searches; j++)
    {
      file >> key;
      ht.search(key);
    }
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
    times.push_back(duration.count());
  }
  file_out << n_searches << ","
           << accumulate(times.begin(), times.end(), 0.0) / times.size()
           << endl;
  file.close();
  file_out.close();
}

void test_insert(unordered_map<int, int> &um, int n_inserts, int n_tests,
                 const char *numbers_file, const char *out_file,
                 bool overwrite = false)
{
  ifstream file(numbers_file);
  ofstream file_out(out_file, overwrite ? ios::trunc : ios::app);

  vector<double> times;
  for (int i = 0; i < n_tests; i++)
  {
    int key;
    auto start = chrono::high_resolution_clock::now();
    for (int j = 0; j < n_inserts; j++)
    {
      file >> key;
      um[key] = 1;
    }
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
    times.push_back(duration.count());
  }

  // Calcular promedio tiempo
  file_out << n_inserts << ","
           << accumulate(times.begin(), times.end(), 0.0) / times.size()
           << endl;
  file.close();
  file_out.close();
}

int main(int argc, char const *argv[])
{
  // ------ TODO ESTO ES DEL CODIGO VIEJO --------
  // const int N = 1000000;
  // HashTable ht_linear(N, linear_probing);
  // HashTable ht_quadratic(N, quadratic_probing);
  // HashTable ht_double(N, double_hashing);
  // unordered_map<int, int> um;

  // cout << "Insertando números aleatorios" << endl;
  // for (int i = 0; i < 9; i++)
  // {
  //   test_insert(ht_linear, pow(2, 10 + i), 1, "random_numbers.txt",
  //               "linear_insert.csv", false);
  //   test_insert(ht_quadratic, pow(2, 10 + i), 1, "random_numbers.txt",
  //               "quadratic_insert.csv", false);
  //   test_insert(ht_double, pow(2, 10 + i), 1, "random_numbers.txt",
  //               "double_hash_insert.csv", false);
  // }

  // cout << "Buscando números aleatorios" << endl;
  // for (int i = 0; i < 9; i++)
  // {
  //   test_search(ht_linear, 1000, 1, "random_numbers.txt", "linear_search.csv",
  //               false);
  //   test_search(ht_linear, 1000, 1, "random_numbers.txt", "quadratic_search.csv",
  //               false);
  //   test_search(ht_linear, 1000, 1, "random_numbers.txt", "double_search.csv",
  //               false);
  // }
  //---------- TERMINO CODIGO VIEJO ----------

  // pasamos todos los datos del CSV a un vector (así es mas sencillo de acceder desde el programa)
  vector<User> users = readCSV("universities_followers.csv");

  // Ahora podemos acceder a los usuarios en forma de struct :O
  for (auto user : users)
  {
    cout << user.userName << endl;
  }
  cout << "cantidad de usuarios " << users.size();

  return 0;
}