#include <chrono>
#include <fstream>
#include <iostream>
#include <math.h>
#include <numeric>
#include <random>
#include <unordered_map>

using namespace std;

// Constante que se utiliza en la función de hash h2
const float A = (sqrt(5) - 1) / 2;

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
  float a = (float)k * A;
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

// Creador de números aleatorios
void generate_random_numbers(int n, int max, const char *filename)
{
  ofstream file(filename);

  // Clase para numeros aleatorios
  random_device rd;
  mt19937 gen(rd());
  uniform_int_distribution<> dis(1, max);

  for (int i = 0; i < n; i++)
  {
    file << dis(gen) << endl;
  }
  file.close();
}

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
  const int N = 1000000;
  HashTable ht_linear(N, linear_probing);
  HashTable ht_quadratic(N, quadratic_probing);
  HashTable ht_double(N, double_hashing);
  unordered_map<int, int> um;

  // Generar números aleatorios

  // cout << "Generando números aleatorios" << endl;
  // generate_random_numbers(N, N, "random_numbers.txt");

  cout << "Insertando números aleatorios" << endl;
  for (int i = 0; i < 9; i++)
  {
    test_insert(ht_linear, pow(2, 10 + i), 1, "random_numbers.txt",
                "linear_insert.csv", false);
    test_insert(ht_quadratic, pow(2, 10 + i), 1, "random_numbers.txt",
                "quadratic_insert.csv", false);
    test_insert(ht_double, pow(2, 10 + i), 1, "random_numbers.txt",
                "double_hash_insert.csv", false);
  }

  cout << "Buscando números aleatorios" << endl;
  for (int i = 0; i < 9; i++)
  {
    test_search(ht_linear, 1000, 1, "random_numbers.txt", "linear_search.csv",
                false);
    test_search(ht_linear, 1000, 1, "random_numbers.txt", "quadratic_search.csv",
                false);
    test_search(ht_linear, 1000, 1, "random_numbers.txt", "double_search.csv",
                false);
  }
  // cout << "tests insert números aleatorios" << endl;
  // test_insert(ht_linear, 1000000, 1, "random_numbers.txt",
  // "linear_insert.csv", true);
  // cout << "end test insert números aleatorios" << endl;

  // cout << "buscando números aleatorios" << endl;
  // test_search(ht_linear, 1000, 1, "random_numbers.txt", "linear_search.csv",
  //             false);
  // cout << "end test buscando números aleatorios" << endl;

  return 0;
}