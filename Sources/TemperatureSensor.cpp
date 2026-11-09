#include <cstdlib>  // Pour rand()

#include "../Headers/TemperatureSensor.h"

TemperatureSensor::TemperatureSensor(Server* server)
    : Sensor("Temperature", server, 5) // <-- 5 secondes ici
{
}

void TemperatureSensor::execute(){
    float valeur = static_cast<float>(rand() % 10001) / 100.0f; // valeur normale entre 0 et 100

    if (doitInjecterAnomalie()){
        valeur = 101.0f + static_cast<float>(rand() % 50); // anomalie : hors de la plage 0-100
    }

    if (server != nullptr){
        server->consoleWrite(getType(), getId(), to_string(valeur));
        server->fileWrite(getType(), getId(), to_string(valeur));
    }
}