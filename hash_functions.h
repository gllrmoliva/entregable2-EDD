#ifndef HASH_FUNCTIONS
#define HASH_FUNCTIONS
#include <string>
#include <cstdint>

using namespace std;

/* Función de hasheo número 1
@param k: clave a la cual aplicaremos la función hash
*/
unsigned int h1(uint64_t k) { return (unsigned int)(k); }

/* Función de hasheo número 2
 @param k: clave a la cual aplicaremos la función hash
 */
unsigned int h2(uint64_t k)
{
    return (unsigned int)((k * 2));
}

/* Aplica la función de hasheo de strings vista en clase (polynomial rolling hash function)
@param str:  palabra a la que se le aplicara la función
*/
unsigned int hash_string(const string &str)
{

    // en esta es la funcion vista en clases, en este caso se utilizo el numero
    //  31 porque es primo y lo recomendaban en diversas fuentes
    unsigned p = 31;
    unsigned int hash_value = 0;
    unsigned int p_pow = 1;
    for (char c : str)
    {
        hash_value += (c - 'a' + 1) * p_pow;
        p_pow *= p;
    }
    return hash_value;
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
    return (h1(k) + i) % n;
}

/* Quadratic probing
@param k: clave a la cual aplicaremos la función hash
@param n: tamaño de la tabla hash
@param i: número del intento
*/
int quadratic_probing(uint64_t k, int n, int i)
{
    // Utilizando el método de la division
    return (h1(k) + i + 2 * i * i) % n;
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
    return (h1(k) + i * (h2(k) + 1)) % n;
}

/* Linear probing
@param k: clave a la cual aplicaremos la función hash
@param n: tamaño de la tabla hash
@param i: número del intento
*/
unsigned int linear_probing(const string &userName, int n, int i)
{
    unsigned k = hash_string(userName);

    // Utilizando el método de la division
    return (h1(k) + i) % n;
}
/* Quadratic probing
@param k: clave a la cual aplicaremos la función hash
@param n: tamaño de la tabla hash
@param i: número del intento
*/
unsigned int quadratic_probing(const string &userName, int n, int i)
{
    unsigned k = hash_string(userName);

    // Utilizando el método de la division
    return (h1(k) + i + 2 * i * i) % n;
}

/* Double hashing
@param k: clave a la cual aplicaremos la función hash
@param n: tamaño de la tabla hash
@param i: número del intento
*/
unsigned int double_hashing(const string &userName, int n, int i)
{
    unsigned k = hash_string(userName);
    // Utilizando como primer método el método de la division y luego el
    // método de la multiplicacion
    return (h1(k) + i * (h2(k) + 1)) % n;
}

#endif