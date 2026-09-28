#include <iostream>

#include "../Headers/Server.h"
#include "../Headers/Sensor.h"
#include "../Headers/Scheduler.h"

using namespace std;

int main()
{
    Server server("Server1");   // Création d'un Server
    
    // Création des Sensors
    Sensor sensor1("Temperature", &server);
    Sensor sensor2("Humidity", &server);
    Sensor sensor3("Sound", &server);

    Scheduler scheduler;    // Création du Scheduler

    // Ajout des Sensors au Scheduler
    scheduler.addSensor(&sensor1);
    scheduler.addSensor(&sensor2);
    scheduler.addSensor(&sensor3);

    
    scheduler.simulation();

    return 0;
}