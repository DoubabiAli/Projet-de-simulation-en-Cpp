#include <iostream>
#include <string>
#include <thread> // Indispensable pour this_thread::sleep_for
#include <chrono>

#include "../Headers/Scheduler.h"

using namespace std;

// implementation de la classe Scheduler

Scheduler::Scheduler(){}

void Scheduler::addSensor(Sensor* sensor){
    sensors.push_back(sensor); // push_back() ajoute l'adresse du capteur à la fin du vector
}

void Scheduler::simulation(){
    while (true) {
        for (size_t i = 0; i < sensors.size(); i++) // On parcourt tous les capteurs présents, sensors.size() affiche le nombre de capteurs présents dans le vector
        {
            sensors[i]->update(); // le Scheduler demande donc à chaque Sensor d'effectuer sa mise à jour
        }
        this_thread::sleep_for(chrono::seconds(1));

    }
}