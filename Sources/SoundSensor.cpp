#include "../Headers/SoundSensor.h"
#include <cstdlib>
#include <string>

SoundSensor::SoundSensor(Server* server)
    : Sensor("Sound", server), valeur(0) {}

void SoundSensor::execute(){
    valeur = rand() % 101;
    envoyerValeur(std::to_string(valeur));
}

int SoundSensor::getValeur() const{
    return valeur;
}