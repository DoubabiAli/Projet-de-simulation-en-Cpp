#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <iostream>
#include <string>
#include <vector>

#include "Sensor.h"

using namespace std;

class Scheduler {  //la class Scheduler qui gere les capteurs et la simulation
    
private:

    vector<Sensor*> sensors; // On utilise un vector et pas un tableau statique car le nombre de capteurs peut varier.

public:

    Scheduler();

    void addSensor(Sensor* sensor); // Ajoute l'adresse d'un capteur dans la liste des capteurs

    void simulation(); 
};

#endif