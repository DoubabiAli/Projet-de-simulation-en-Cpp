#include <iostream>
#include <string>
#include <cstdlib>  // Pour rand()

#include "../Headers/Sensor.h"

using namespace std;

// implementation de la classe Sensor

int Sensor::idSuivant = 0; // genere un id unique pour chaque Sensor

Sensor::Sensor(const string& type, Server* server){
    sensorId = ++idSuivant;

    sensorType = type;

    this->server = server;
}

Sensor::~Sensor(){}

void Sensor::update(){
    execute();
}

void Sensor::envoyerValeur(const string& valeur) const{
    if (server != nullptr){  // Verification que le Sensor possède bien un Server.
        
        server->consoleWrite(sensorType, sensorId, valeur);

        server->fileWrite(sensorType, sensorId, valeur);
    }
    else{
        cout << "Erreur : " << sensorId << " n'a pas de Server." << endl;
    }
}

// Getters

int Sensor::getId() const
{
    return sensorId;
}

string Sensor::getType() const
{
    return sensorType;
}