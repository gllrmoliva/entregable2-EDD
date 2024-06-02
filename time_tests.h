#ifndef TIME_TESTS
#define TIME_TESTS

#include <fstream>
#include <chrono>
#include <numeric>
#include <unordered_map>
#include <variant>

#include "hash_functions.h"
#include "hash_tables.h"
#include "functions.h"

using namespace std;
using namespace std::chrono;

/**
 * @enum HashTableType
 * @brief Enumera los diferentes tipos de tablas hash utilizadas en la aplicación.
 *
 * Este enum define varios tipos de tablas hash que pueden ser utilizados 
 * para almacenar y buscar usuarios, ya sea por ID o por nombre, utilizando 
 * diferentes métodos de resolución de colisiones.
 */
enum HashTableType
{
    user_id_open,
    user_id_close,
    user_name_open,
    user_name_close,
    unordered_map_by_name,
    unordered_map_by_id,
};

//----------------------------------------------------------------------//
//----------------------------TESTS DE INSERT---------------------------//
//----------------------------------------------------------------------//

/**
 * @brief Dado una base de datos de usuarios en forma de vector, inserta n_inserts usuarios a una tabla hash elegida y
 * calcula el tiempo que demora hacer esta operación.
 *
 * @param type: tipo de tabla hash (del tipo key-User), los valores son parte de un enumerador.
 * @param max_size: tamaño máximo de la tabla hash.
 * @param users: Vector con datos sobre usuarios, desde aqui se sacaran los usuarios a añadir.
 * @param n_inserts: Cantidad de usuarios a insertar.
 * @param hashing_method_id: puntero a una funcion de hashing para utilizar con userId. por defecto es nullptr
 * @param hashing_method_name: puntero a una funcion de hashing para utilizar con userName. por defecto es nullptr
 *
 */
double test_insert(HashTableType type, int max_size, vector<User> &users, int n_inserts,
                   int (*hashing_method_id)(unsigned long long, int, int) = nullptr,
                   unsigned int (*hashing_method_name)(const string &, int, int) = nullptr)
{
    auto start = chrono::high_resolution_clock::now();

    switch (type)
    {
    case user_id_open:
    {
        OpenHashTableUserId hash_table(max_size);
        for (int i = 0; i < n_inserts; i++)
        {
            hash_table.insert(users[i].userId, &users[i]);
        }
        break;
    }
    case user_name_open:
    {
        OpenHashTableUserName hash_table(max_size);
        for (int i = 0; i < n_inserts; i++)
        {
            hash_table.insert(users[i].userName, &users[i]);
        }
        break;
    }
    case unordered_map_by_name:
    {
        unordered_map<string, User> hash_table;
        for (int i = 0; i < n_inserts; i++)
        {
            hash_table[users[i].userName] = users[i];
        }
        break;
    }
    case unordered_map_by_id:
    {
        unordered_map<int64_t, User> hash_table;
        for (int i = 0; i < n_inserts; i++)
        {
            hash_table[users[i].userId] = users[i];
        }
        break;
    }
    case user_id_close:
    {
        if (hashing_method_id == nullptr)
        {
            cout << "el hashing_method para user_id_close no fue proporcionado." << endl;
            return 0;
        }
        CloseHashTableUserId hash_table(max_size, hashing_method_id);
        for (int i = 0; i < n_inserts; i++)
        {
            hash_table.insert(users[i].userId, &users[i]);
        }
        break;
    }
    case user_name_close:
    {
        if (hashing_method_name == nullptr)
        {
            cout << "el hashing_method para user_id_close no fue proporcionado." << endl;
            return 0;
        }
        CloseHashTableUserName hash_table(max_size, hashing_method_name);
        for (int i = 0; i < n_inserts; i++)
        {
            hash_table.insert(users[i].userName, &users[i]);
        }
        break;
    }
    default:
        cout << "Algo fallo en la función, asegurate de poner todos los parametros" << endl;
        return 0;
    }

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;

    return duration.count();
}
/**
 * @brief Aplica la función test_insert() a todas las tablas hash con key username una cantidad
 * de veces seleccionada por el usuario, los datos guardan de forma externa en un archivo con extención .csv.
 * En este archivo se guardan los datos de los experimentos en el siguiente orden: tipo de hasheo, número de inserts, tiempo.
 *
 * @param n_tests: cantidad de test a ejecutar.
 * @param users: usuarios los cuales se insertaran a las tablas hash.
 * @param table_size: tamaño de las tablas a insertar datos.
 * @param file_name: nombre del archivo saliente, este se pone sin la extension.
 *
 * @note Todas las tablas a testear tienen el mismo tamaño.
 */
void test_inserts_by_username(int n_tests, vector<User> users, int table_size, string file_name)
{
    int n_inserts[] = {1000, 5000, 10000, 15000, 20000};
    ofstream file_out(file_name + ".csv", false ? ios::trunc : ios::app);
    file_out << "Tipo de hasheo, Número de inserciones, Tiempo(s)" << endl;
    for (int inserts : n_inserts)
    {
        for (int i = 0; i < n_tests; i++)
        {
            file_out << "lineal probing," << inserts << ",";
            file_out << test_insert(user_name_close, table_size, users, inserts, nullptr, linear_probing) << endl;
            file_out << "double hashing," << inserts << ",";
            file_out << test_insert(user_name_close, table_size, users, inserts, nullptr, double_hashing) << endl;
            file_out << "quadratic probing," << inserts << ",";
            file_out << test_insert(user_name_close, table_size, users, inserts, nullptr, quadratic_probing) << endl;
            file_out << "chaining," << inserts << ",";
            file_out << test_insert(user_name_open, table_size, users, inserts) << endl;
            file_out << "STL unordered map," << inserts << ",";
            file_out << test_insert(unordered_map_by_name, table_size, users, inserts) << endl;
        }
    }
    file_out.close();
}
/**
 * @brief Aplica la función test_insert() a todas las tablas hash con key userid una cantidad
 * de veces seleccionada por el usuario, los datos guardan de forma externa en un archivo con extención .csv.
 * En este archivo se guardan los datos de los experimentos en el siguiente orden: tipo de hasheo, número de inserts, tiempo.
 *
 * @param n_tests: cantidad de tests a ejecutar.
 * @param users: usuarios los cuales se insertaran a las tablas hash.
 * @param table_size: tamaño de las tablas a insertar datos.
 * @param file_name: nombre del archivo saliente, este se pone sin la extension.
 *
 * @note Todas las tablas a testear tienen el mismo tamaño.
 */
void test_inserts_by_userid(int n_tests, vector<User> users, int table_size, string file_name)
{
    int n_inserts[] = {1000, 5000, 10000, 15000, 20000};
    ofstream file_out(file_name + ".csv", ios::app);
    file_out << "Tipo de hasheo, Número de inserciones, Tiempo(s)" << endl;
    for (int inserts : n_inserts)
    {
        for (int i = 0; i < n_tests; i++)
        {
            file_out << "lineal probing," << inserts << ",";
            file_out << test_insert(user_id_close, table_size, users, inserts, linear_probing, nullptr) << endl;
            file_out << "double hashing," << inserts << ",";
            file_out << test_insert(user_id_close, table_size, users, inserts, double_hashing, nullptr) << endl;
            file_out << "quadratic probing," << inserts << ",";
            file_out << test_insert(user_id_close, table_size, users, inserts, quadratic_probing, nullptr) << endl;
            file_out << "chaining," << inserts << ",";
            file_out << test_insert(user_id_open, table_size, users, inserts) << endl;
            file_out << "STL unordered map," << inserts << ",";
            file_out << test_insert(unordered_map_by_id, table_size, users, inserts) << endl;
        }
    }
    file_out.close();
}

//----------------------------------------------------------------------//
//----------------------------TESTS DE SEARCH---------------------------//
//----------------------------------------------------------------------//

// En test seach no tiene mucho sentido hacer un switch que construya tablas hash, por lo que
// En este caso solo se hara override en la función

/**
 * @brief Calcula la cantidad de tiempo que demora buscar una cantidad de User's dada por el usuario
 * @param hash_table: Tabla hash la cual ya posee datos dentro de sí
 * @param users_to_search: Usuarios que se usaran para las busquedas
 * @param n_searchs: Cantidad de busquedas que se haran en el test.
 */
double test_search(CloseHashTableUserId &hash_table, vector<User> users_to_search, int n_searchs)
{
    auto start = chrono::high_resolution_clock::now();

    for (int i = 0; i < n_searchs; i++)
    {
        hash_table.search(users_to_search[i].userId);
    }

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;

    return duration.count();
}

/**
 * @brief Calcula la cantidad de tiempo que demora buscar una cantidad de User's dada por el usuario
 * @param hash_table: Tabla hash la cual ya posee datos dentro de sí
 * @param users_to_search: Usuarios que se usaran para las busquedas
 * @param n_searchs: Cantidad de busquedas que se haran en el test.
 */
double test_search(OpenHashTableUserId &hash_table, vector<User> users_to_search, int n_searchs)
{
    auto start = chrono::high_resolution_clock::now();

    for (int i = 0; i < n_searchs; i++)
    {
        hash_table.search(users_to_search[i].userId);
    }

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;

    return duration.count();
}

/**
 * @brief Calcula la cantidad de tiempo que demora buscar una cantidad de User's dada por el usuario
 * @param hash_table: Tabla hash la cual ya posee datos dentro de sí
 * @param users_to_search: Usuarios que se usaran para las busquedas
 * @param n_searchs: Cantidad de busquedas que se haran en el test.
 */
double test_search(unordered_map<unsigned long long, User> &hash_table, vector<User> users_to_search, int n_searchs)
{
    auto start = chrono::high_resolution_clock::now();

    for (int i = 0; i < n_searchs; i++)
    {
        hash_table.find(users_to_search[i].userId);
    }

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;

    return duration.count();
}

/**
 * @brief Calcula la cantidad de tiempo que demora buscar una cantidad de User's dada por el usuario
 * @param hash_table: Tabla hash la cual ya posee datos dentro de sí
 * @param users_to_search: Usuarios que se usaran para las busquedas
 * @param n_searchs: Cantidad de busquedas que se haran en el test.
 */
double test_search(CloseHashTableUserName &hash_table, vector<User> users_to_search, int n_searchs)
{
    auto start = chrono::high_resolution_clock::now();

    for (int i = 0; i < n_searchs; i++)
    {
        hash_table.search(users_to_search[i].userName);
    }

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;

    return duration.count();
}

/**
 * @brief Calcula la cantidad de tiempo que demora buscar una cantidad de User's dada por el usuario
 * @param hash_table: Tabla hash la cual ya posee datos dentro de sí
 * @param users_to_search: Usuarios que se usaran para las busquedas
 * @param n_searchs: Cantidad de busquedas que se haran en el test.
 */
double test_search(OpenHashTableUserName &hash_table, vector<User> users_to_search, int n_searchs)
{
    auto start = chrono::high_resolution_clock::now();

    for (int i = 0; i < n_searchs; i++)
    {
        hash_table.search(users_to_search[i].userName);
    }

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;

    return duration.count();
}

/**
 * @brief Calcula la cantidad de tiempo que demora buscar una cantidad de User's dada por el usuario
 * @param hash_table: Tabla hash la cual ya posee datos dentro de sí
 * @param users_to_search: Usuarios que se usaran para las busquedas
 * @param n_searchs: Cantidad de busquedas que se haran en el test.
 */
double test_search(unordered_map<string, User> &hash_table, vector<User> users_to_search, int n_searchs)
{
    auto start = chrono::high_resolution_clock::now();

    for (int i = 0; i < n_searchs; i++)
    {
        hash_table.find(users_to_search[i].userName);
    }

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;

    return duration.count();
}

/**
 * @brief Aplica la función test_search() a todas las tablas hash con key username una cantidad
 * de veces seleccionada por el usuario, los datos guardan de forma externa en un archivo con extención .csv.
 * En este archivo se guardan los datos de los experimentos en el siguiente orden: tipo de hasheo, número de busquedas, tiempo.
 *
 * @param n_tests: cantidad de tests a ejecutar.
 * @param users_in_tables: usuarios los cuales estaran dentro de las tablas.
 * @param users_to_search: usuarios los cuales se buscaran dentro de las tablas.
 * @param table_size: tamaño de las tablas a insertar datos.
 * @param file_name: nombre del archivo saliente, este se pone sin la extension.
 *
 * @note Todas las tablas a testear tienen el mismo tamaño.
 */
void test_searchs_by_username(int n_tests, vector<User> users_in_tables, vector<User> users_to_search,
                              int table_size, string file_name)
{
    CloseHashTableUserName linear_table(table_size, linear_probing);
    CloseHashTableUserName double_table(table_size, double_hashing);
    CloseHashTableUserName quadratic_table(table_size, quadratic_probing);
    OpenHashTableUserName chaining_table(table_size);
    unordered_map<string, User> STL_table(table_size);

    // rellenemos las tablas con datos
    for (User user : users_in_tables)
    {
        linear_table.insert(user.userName, &user);
        double_table.insert(user.userName, &user);
        quadratic_table.insert(user.userName, &user);
        chaining_table.insert(user.userName, &user);
        STL_table[user.userName] = user;
    }

    int n_searchs[] = {1000, 5000, 10000, 15000, 20000};

    ofstream file_out(file_name + ".csv", false ? ios::trunc : ios::app);

    file_out << "Tipo de hasheo, Número de busquedas, Tiempo(s)" << endl;

    for (int searchs : n_searchs)
    {
        for (int j = 0; j < n_tests; j++)
        {
            file_out << "lineal probing," << searchs << ",";
            file_out << test_search(linear_table, users_to_search, searchs) << endl;
            file_out << "double hashing," << searchs << ",";
            file_out << test_search(double_table, users_to_search, searchs) << endl;
            file_out << "quadratic probing," << searchs << ",";
            file_out << test_search(quadratic_table, users_to_search, searchs) << endl;
            file_out << "chaining," << searchs << ",";
            file_out << test_search(chaining_table, users_to_search, searchs) << endl;
            file_out << "STL unordered map," << searchs << ",";
            file_out << test_search(STL_table, users_to_search, searchs) << endl;
        }
    }
    file_out.close();
}

/**
 * @brief Aplica la función test_search() a todas las tablas hash con key userid una cantidad
 * de veces seleccionada por el usuario, los datos guardan de forma externa en un archivo con extención .csv.
 * En este archivo se guardan los datos de los experimentos en el siguiente orden: tipo de hasheo, número de busquedas, tiempo.
 *
 * @param n_tests: cantidad de tests a ejecutar.
 * @param users_in_tables: usuarios los cuales estaran dentro de las tablas.
 * @param users_to_search: usuarios los cuales se buscaran dentro de las tablas.
 * @param table_size: tamaño de las tablas a insertar datos.
 * @param file_name: nombre del archivo saliente, este se pone sin la extension.
 *
 * @note Todas las tablas a testear tienen el mismo tamaño.
 */
void test_searchs_by_userid(int n_tests, vector<User> users_in_tables, vector<User> users_to_search,
                            int table_size, string file_name)
{
    CloseHashTableUserId linear_table(table_size, linear_probing);
    CloseHashTableUserId double_table(table_size, double_hashing);
    CloseHashTableUserId quadratic_table(table_size, quadratic_probing);
    OpenHashTableUserId chaining_table(table_size);
    unordered_map<unsigned long long, User> STL_table(table_size);

    // rellenemos las tablas con datos
    for (User user : users_in_tables)
    {
        linear_table.insert(user.userId, &user);
        double_table.insert(user.userId, &user);
        quadratic_table.insert(user.userId, &user);
        chaining_table.insert(user.userId, &user);
        STL_table[user.userId] = user;
    }

    int n_searchs[] = {1000, 5000, 10000, 15000, 20000};

    ofstream file_out(file_name + ".csv", ios::app);

    file_out << "Tipo de hasheo, Número de busquedas, Tiempo(s)" << endl;

    for (int searchs : n_searchs)
    {
        for (int j = 0; j < n_tests; j++)
        {
            file_out << "lineal probing," << searchs << ",";
            file_out << test_search(linear_table, users_to_search, searchs) << endl;
            file_out << "double hashing," << searchs << ",";
            file_out << test_search(double_table, users_to_search, searchs) << endl;
            file_out << "quadratic probing," << searchs << ",";
            file_out << test_search(quadratic_table, users_to_search, searchs) << endl;
            file_out << "chaining," << searchs << ",";
            file_out << test_search(chaining_table, users_to_search, searchs) << endl;
            file_out << "STL unordered map," << searchs << ",";
            file_out << test_search(STL_table, users_to_search, searchs) << endl;
        }
    }
    file_out.close();
}

#endif
