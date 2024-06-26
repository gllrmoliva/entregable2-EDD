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

int main()
{
  /*
  Primero guardamos los datos en los CSV dentro de vectores, exite uno para usuarios que no estaran en la base
  de datos y otros los cuales si se encontraran. Los que no se encontraran fueron generados con un
  script de python. "create_data"
  */
  /*
  Notemos además que se genero otro archivo, con los seguidores de universidades, esto ya que habian usuarios repetidos.
  Se eliminaron los repetidos con un script de python "delete_duplicates"
  */
  vector<User> real_users = readCSV("universities_followers_without_duplicates.csv");
  vector<User> fake_users = readCSV("fake_data.csv");

  // Tamaño de la tabla, fue elegido ya que es un número primo el cual es cercano al factor de carga muy alto, esto para comparar colisiones
  const int table_size = 21089;

  // Cantidad de test que se haran
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

  // Calculo de colisiones y memoria utilizada.
  int tests[] = {1000, 2500, 5000, 10000, 12500, 15000, 17500, 19908};
  for (int i : tests)
  {
    memory_test(table_size, i, real_users, "tests/test_de_memory");
    colisions_test(table_size, i, real_users, "tests/test_colisiones");
  }

  return 0;
}
