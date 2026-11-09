#include <string>
#include <cstdlib>  // Pour rand()

#include "../Headers/Sensor.h"

using namespace std;

// implementation de la classe Sensor

int Sensor::idSuivant = 0; // genere un id unique pour chaque Sensor

bool Sensor::injectAnomalies = false; // mode test désactivé par défaut

Sensor::Sensor(const string& type, Server* server, int periode)
    : periode(periode), compteur(0){
    sensorId = ++idSuivant;

    sensorType = type;
    this->server = server;
}

Sensor::~Sensor(){}

void Sensor::update(){
    ++compteur;
    if (compteur >= periode) {
        execute();
        compteur = 0;
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

bool Sensor::doitInjecterAnomalie() const
{
    // Si le mode test est désactivé, rand() n'est pas appelé :
    // le fonctionnement normal du simulateur reste exactement le même.
    return injectAnomalies && rand() % 50 == 0;
}