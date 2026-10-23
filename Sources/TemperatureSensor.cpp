#include "../Headers/TemperatureSensor.h"

TemperatureSensor::TemperatureSensor(Server* server)
    : Sensor("Temperature", server, 5) // <-- 5 secondes ici
{
}
void TemperatureSensor::execute(){
    float valeur = static_cast<float>(rand() % 10001) / 100.0f;

    if (server != nullptr){
        server->consoleWrite(getType(), getId(), to_string(valeur));
        server->fileWrite(getType(), getId(), to_string(valeur));
    }
}

