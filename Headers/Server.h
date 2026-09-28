#ifndef SERVER_H // Protection contre les inclusions multiples
#define SERVER_H // Empêcher le contenu d'un fichier .h d'être inclus plusieurs fois dans la même compilation.

#include <iostream>
#include <string>
#include <fstream> //Pour l'ecriture sur les fichiers

using namespace std;

class Server { //la class serveur qui recoit les donnees

private:

    string nomServer;

public:

    Server(); //constructeur par defaut

    Server(const string& nom); //constructeur avec un nom 

    Server(const Server& other); //constructeur de recopie

    Server& operator=(const Server& other); //operateur d'affectation

    ~Server(); //destructeur

    void consoleWrite( //affichage des mesures sur la console
        const string& sensorType, 
        int sensorId, 
        const string& valeur);

    void fileWrite( //ecriture des mesures sur un fichier CSV
        const string& sensorType, 
        int sensorId, 
        const string& valeur);

    string getName(); //retourne le nom du serveur car il est private et ne peut pas etre accede directement

};

#endif