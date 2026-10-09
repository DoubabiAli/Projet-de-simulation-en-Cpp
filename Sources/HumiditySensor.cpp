#include "../Headers/HumiditySensor.h"
#include <cstdlib>
#include <string>

HumiditySensor::HumiditySensor(Server* server)
    : Sensor("Humidity", server), valeur(0.0f) {}

void HumiditySensor::execute(){
    valeur = static_cast<float>(rand() % 10001) / 100.0f;
    envoyerValeur(std::to_string(valeur));
}

float HumiditySensor::getValeur() const{
    return valeur;
}