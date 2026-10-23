#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <filesystem>

#include "../Headers/Server.h"

using namespace std;

// implementation de la classe Server

Server::Server(){
    nomServer = "Server";
}

Server::Server(const string& nom){
    nomServer = nom;
}

Server::Server(const Server& other){
    nomServer = other.nomServer;
}

Server& Server::operator=(const Server& other){
    if (this != &other){
        nomServer = other.nomServer;
    }

    return *this;
}

Server::~Server(){}

void Server::consoleWrite(const string& sensorType,int sensorId, const string& valeur){
    cout << "Server = " << nomServer << " ;; Sensor = " << sensorId << " ;; Type = " << sensorType << " ;; Valeur = " << valeur << endl;
}

void Server::fileWrite(const string& sensorType,int sensorId,const string& valeur){
    string nomFile = sensorType + ".csv";
    // L'en-tete est ecrit uniquement si le fichier est nouveau ou vide.
    bool fichierVide = !filesystem::exists(nomFile) || filesystem::file_size(nomFile) == 0;
    ofstream fichier(nomFile, ios::app);

    if (fichier.is_open()){
        if (fichierVide){
            fichier << "Date;Heure;SensorId;SensorType;Valeur" << endl;
        }

        time_t maintenant = time(nullptr);
        tm* dateLocale = localtime(&maintenant);

        fichier << put_time(dateLocale, "%Y-%m-%d") << ";" << put_time(dateLocale, "%H:%M:%S") << ";" << sensorId << ";" << sensorType << ";" << valeur << endl;
        fichier.close();
    }
    else{
        cout << "Erreur de fichier : " << nomFile << endl;
    }
}

string Server::getName(){
    return nomServer;
}