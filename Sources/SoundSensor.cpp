#include "../Headers/SoundSensor.h"

SoundSensor::SoundSensor(Server* server)
    : Sensor("Sound", server)
{
}

void SoundSensor::execute(){
    int valeur = rand() % 101; // genere une valeur aléatoire entre 0 et 100

    if (server != nullptr){
        server->consoleWrite(getType(), getId(), to_string(valeur));
        server->fileWrite(getType(), getId(), to_string(valeur));
    }
}