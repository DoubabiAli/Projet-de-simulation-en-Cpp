#include "../Headers/LightSensor.h"
#include <cstdlib>
#include <string>

LightSensor::LightSensor(Server* server)
    : Sensor("Light", server)
{
}

void LightSensor::execute(){

    bool light = rand() % 2;

    string valeur;

    if (light)
    {
        valeur = "true";
    }
    else
    {
        valeur = "false";
    }

    if (server != nullptr)
    {
        server->consoleWrite(getType(), getId(), valeur);
        server->fileWrite(getType(), getId(), valeur);
    }
}
