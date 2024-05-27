#ifndef FUNCTIONS
#define FUNCTIONS

#include <cmath>
#include <string>
#include <iostream>
#include <cstdint>
#include <vector>
#include <sstream>
#include <fstream>

using namespace std;

uint64_t scientificToNormal(const std::string &scientific)
{
    std::istringstream iss(scientific);
    double number;

    iss >> number; // Convertir la cadena a un numero en punto flotante
    uint64_t normal = static_cast<uint64_t>(number);
    return normal;
}
/*
Struct que guarda los datos de un usuario.
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

    /*
    constructor por defecto de User, si se crea un User, utilizado cuando no se especifica el valor
    de un parametro.
    */
    User() : university(""), userId(0), userName(""), numberTweets(0), friendsCount(0), followersCount(0), createdAt("")
    {
    }

    /*
    Contructor de struct User.
    @param uni: universidad a la que sigue.
    @param id: identificador del usuario
    @param name: nombre de usuario.
    @param tweets: cantidad de tweets.
    @param friends: cantidad de amigos.
    @param followers: número de seguidores.
    @param created: fecha de creación de la cuenta.
    */
    User(const string &uni, uint64_t id, const string &name, int tweets, int friends, int followers, const string &created)
        : university(uni), userId(id), userName(name), numberTweets(tweets), friendsCount(friends), followersCount(followers), createdAt(created) {}
};

/*
Carga los datos del CSV y los pasa a un vector de la STL
@param filename: nombre del archivo con extención.
@return Vector con todos los usuarios cargados satisfactoriamente
@note Referencia: https://www.geeksforgeeks.org/how-to-read-data-from-csv-file-to-a-2d-array-in-cpp/
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
            uint64_t userId = scientificToNormal(tokens[1]); // Convertir a numero normal
            string userName = tokens[2];
            int numberTweets = stoi(tokens[3]);
            int friendsCount = stoi(tokens[4]);
            int followersCount = stoi(tokens[5]);
            string createdAt = tokens[6];

            // std::cout << "Usuario leído: " << userName << ", userId: " << userId << endl;

            // emplace_back esta buena, no necesitas crear por fuera el objeto, lo crea solo, pero obviamente
            // le tienes que dar los parametros
            users.emplace_back(university, userId, userName, numberTweets, friendsCount, followersCount, createdAt);
        }
    }

    std::cout << "Leidos " << users.size() << " usuarios del archivo CSV." << endl;

    return users;
}

/*Imprime en la consola los datos de un User
@param foundUser: User del cual se mostraran los datos
*/
void printUser(User *foundUser)
{
    if (foundUser)
    {
        std::cout << "Nombre Usuario: " << foundUser->userName << endl;
        std::cout << "ID: " << foundUser->userId << endl;
        std::cout << "Universidad: " << foundUser->university << endl;
        std::cout << "Numero de Tweets: " << foundUser->numberTweets << endl;
        std::cout << "Numero de amigos: " << foundUser->friendsCount << endl;
        std::cout << "Numero de seguidores: " << foundUser->followersCount << endl;
        std::cout << "Fecha de creacion: " << foundUser->createdAt << endl;
    }
    else
    {
        std::cout << "Usuario no encontrado" << endl;
    }
}

#endif