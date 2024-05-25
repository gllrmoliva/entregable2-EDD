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
  return users;
}

//--- Funciones hash ---

/* Método de la división
- k: clave a la cual aplicaremos la función hash
- n: tamaño de la tabla hash
*/
int h1(int k, int n) { return k % n; }

/* Método de la multiplicación
- k: clave a la cual aplicaremos la función hash
- n: tamaño de la tabla hash */
int h2(int k, int n)
{
  return k * 53 + k * k * 13 + 18;
}

//--- Métodos de Open addressing o hashing cerrado ---

/* Linear probing
- k: clave a la cual aplicaremos la función hash
- n: tamaño de la tabla hash
- i: número del intento
*/
int linear_probing(int k, int n, int i)
{
  // Utilizando el método de la division
  return (h1(k, n) + i) % n;
}

/* Quadratic probing
- k: clave a la cual aplicaremos la función hash
- n: tamaño de la tabla hash
- i: número del intento
*/
int quadratic_probing(int k, int n, int i)
{
  // Utilizando el método de la division
  return (h1(k, n) + i + 2 * i * i) % n;
}

/* Double hashing
- k: clave a la cual aplicaremos la función hash
- n: tamaño de la tabla hash
- i: número del intento */
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
  vector<User> users = readCSV("test_data_copied.csv");

  const int N = 42157;
  HashTableUserName ht_test(N, double_hashing_username);

  for (auto &user : users)
  {
    ht_test.insert(user.userName, &user);
  }
  cout << "hola" << endl;

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

  return 0;
}

/* TO-DO list
- Hacer que al hacer remove queden "marcadas las casillas"
- Hacer la hash table con STL
- Hacer la hash table abierta (vectores¿?)
- Separar en archivos: hash_tables, tests, hash_functions, main
*/