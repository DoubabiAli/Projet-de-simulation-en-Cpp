#include <iostream>
#include <string>
#include <fstream>

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
    ofstream fichier(nomFile, ios::app);

    if (fichier.is_open()){
        fichier << sensorId << "," << valeur << endl;
        fichier.close();
    }
    else{
        cout << "Erreur de fichier : " << nomFile << endl;
    }
}

string Server::getName(){
    return nomServer;
}