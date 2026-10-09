#include "../Headers/TemperatureSensor.h"
#include <cstdlib>
#include <string>

TemperatureSensor::TemperatureSensor(Server* server)
    : Sensor("Temperature", server), valeur(0.0f) {}

void TemperatureSensor::execute(){
    valeur = static_cast<float>(rand() % 10001) / 100.0f;
    envoyerValeur(std::to_string(valeur));
}

float TemperatureSensor::getValeur() const{
    return valeur;
}