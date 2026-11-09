#include <iostream>
#include <cstdlib>
#include <ctime>
#include <filesystem>
#include <stdexcept>
#include <thread>

#include "../Headers/Server.h"
#include "../Headers/Sensor.h"
#include "../Headers/TemperatureSensor.h"
#include "../Headers/HumiditySensor.h"
#include "../Headers/SoundSensor.h"
#include "../Headers/LightSensor.h"
#include "../Headers/Scheduler.h"
#include "../Headers/HttpServer.h"

using namespace std;

int main(int argc, char* argv[])
{
    // Lecture des arguments : seul --inject-anomalies est reconnu
    for (int i = 1; i < argc; i++)
    {
        string argument = argv[i];
        if (argument == "--inject-anomalies")
        {
            Sensor::injectAnomalies = true;
            cout << "Mode test : injection d'anomalies activee" << endl;
        }
        else
        {
            cerr << "Argument inconnu : " << argument << endl;
            cerr << "Usage : " << argv[0] << " [--inject-anomalies]" << endl;
            return 1;
        }
    }

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

    const filesystem::path projectDirectory = filesystem::current_path();
    const filesystem::path frontendDirectory = projectDirectory / "Frontend";
    filesystem::current_path(projectDirectory);
    HttpServer httpServer(frontendDirectory.string(), projectDirectory.string());

    thread simulationThread(&Scheduler::simulation, &scheduler);
    httpServer.run();
    simulationThread.join();

    return 0;
}