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
    valeur = 0.0f;
    this->server = server;
}

Sensor::~Sensor(){}

void Sensor::update(){
    execute();
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

float Sensor::getValeur() const
{
    return valeur;
}