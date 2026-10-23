#include <iostream>
#include <cstdlib>
#include <ctime>

#include "../Headers/Server.h"
#include "../Headers/Sensor.h"
#include "../Headers/TemperatureSensor.h"
#include "../Headers/HumiditySensor.h"
#include "../Headers/SoundSensor.h"
#include "../Headers/LightSensor.h"
#include "../Headers/Scheduler.h"

using namespace std;

int main()
{
    srand(static_cast<unsigned int>(time(nullptr)));

    Server server("Server1");   // Création d'un Server
    
    // Création des Sensors
    TemperatureSensor sensor1(&server);
    HumiditySensor sensor2(&server);
    SoundSensor sensor3(&server);
    LightSensor sensor4(&server);

    Scheduler scheduler;    // Création du Scheduler

    // Ajout des Sensors au Scheduler
    scheduler.addSensor(&sensor1);
    scheduler.addSensor(&sensor2);
    scheduler.addSensor(&sensor3);
    scheduler.addSensor(&sensor4);

    scheduler.simulation();

    return 0;
}