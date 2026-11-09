#include <cstdlib>  // Pour rand()

#include "../Headers/HumiditySensor.h"

HumiditySensor::HumiditySensor(Server* server)
    : Sensor("Humidity", server, 20) // <-- 20 secondes ici
{
}

void HumiditySensor::execute(){

    float valeur = static_cast<float>(rand() % 10001) / 100.0f; // valeur normale entre 0 et 100

    if (doitInjecterAnomalie()){
        valeur = 101.0f + static_cast<float>(rand() % 5000) / 100.0f; // anomalie : de 101 à 150, hors de la plage 0-100
    }

    if (server != nullptr){
        server->consoleWrite(getType(), getId(), to_string(valeur));
        server->fileWrite(getType(), getId(), to_string(valeur));
    }
}