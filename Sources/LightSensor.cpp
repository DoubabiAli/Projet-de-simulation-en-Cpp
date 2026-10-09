#include "../Headers/LightSensor.h"
#include <cstdlib>
#include <string>

LightSensor::LightSensor(Server* server)
    : Sensor("Light", server), valeur(false) {}

void LightSensor::execute(){
    valeur = rand() % 2 == 1;
    envoyerValeur(valeur ? "true" : "false");
}

bool LightSensor::getValeur() const{
    return valeur;
}