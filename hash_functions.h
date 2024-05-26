#ifndef HASH_FUNCTIONS
#define HASH_FUNCTIONS
#include <string>
#include <cstdint>

using namespace std;

/* Función de hasheo número 1
@param k: clave a la cual aplicaremos la función hash
@param n: tamaño de la tabla hash
*/
int h1(uint64_t k, int n) { return k % n; }

/* Función de hasheo número 2
 @param k: clave a la cual aplicaremos la función hash
 @param n: tamaño de la tabla hash
 */
int h2(uint64_t k, int n)
{
    return k * 53 + k * k * 13 + 18;
}

/* Aplica la función de hasheo de strings vista en clase (polynomial rolling hash function)
@param str:  palabra a la que se le aplicara la función
*/
int hash_string(const string &str)
{
    // TODO: revisar esta función (quizas añadir modulo)

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

//--- Métodos de Open addressing o hashing cerrado ---

/* Linear probing
@param k: clave a la cual aplicaremos la función hash
@param n: tamaño de la tabla hash
@param i: número del intento
*/
int linear_probing(uint64_t k, int n, int i)
{
    // Utilizando el método de la division
    return (h1(k, n) + i) % n;
}

/* Quadratic probing
@param k: clave a la cual aplicaremos la función hash
@param n: tamaño de la tabla hash
@param i: número del intento
*/
int quadratic_probing(uint64_t k, int n, int i)
{
    // Utilizando el método de la division
    return (h1(k, n) + i + 2 * i * i) % n;
}

/* Double hashing
@param k: clave a la cual aplicaremos la función hash
@param n: tamaño de la tabla hash
@param i: número del intento
*/
int double_hashing(uint64_t k, int n, int i)
{
    // Utilizando como primer método el método de la division y luego el
    // método de la multiplicacion
    return (h1(k, n) + i * (h2(k, n) + 1)) % n;
}

// TODO: quizas cambiar las funciones _username por un overloading de las funciones anteriores
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

#endif