#include "../Headers/HumiditySensor.h"


HumiditySensor::HumiditySensor(Server* server)
    : Sensor("Humidity", server)
{
}

void HumiditySensor::execute(){
    float valeur = static_cast<float>(rand() % 10001) / 100.0f;
    

    if (server != nullptr){
        server->consoleWrite(getType(), getId(), to_string(valeur));
        server->fileWrite(getType(), getId(), to_string(valeur));
    }
}