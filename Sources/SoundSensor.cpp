#include <cstdlib>  // Pour rand()

#include "../Headers/SoundSensor.h"

SoundSensor::SoundSensor(Server* server)
    : Sensor("Sound", server, 10) // <-- 10 secondes ici
{
}

void SoundSensor::execute(){
    int valeur = rand() % 101; // genere une valeur aléatoire entre 0 et 100

    if (doitInjecterAnomalie()){
        valeur = 101 + rand() % 50; // anomalie : hors de la plage 0-100
    }

    if (server != nullptr){
        server->consoleWrite(getType(), getId(), to_string(valeur));
        server->fileWrite(getType(), getId(), to_string(valeur));
    }
}