#ifndef SENSOR_H // IF N DEFined <=> SI SENSOR_H N'EST PAS défini 
//Est-ce que SERVER_H existe déjà ? Si NON => on lit le contenu | Si OUI => on ignore le contenu
#define SENSOR_H // <=> SI SENSOR_H n'est pas défini on le define

// Le compilateur lit le contenu du fichier .h et marque SENSOR_H comme défini.
// Quand le compilateur rencontre à nouveau le même fichier :
// Est-ce que SENSOR_H est déjà défini ? Oui.
// Donc : "Je l'ai déjà lu, pas besoin de le relire."

#include <string>
#include <iostream>
#include "Server.h"

using namespace std;

class Sensor{

private:
    int sensorId;
    string sensorType;
protected:

    Server* server;

    static int idSuivant;  // Génère des id différents
    
//chageent pour la gestion du temps
    int periode;
    int compteur;

public:
//changement, ajout de int periode 1
    Sensor(const string& type, Server* server,int periode = 1);

    virtual ~Sensor();

    void update();

    virtual void execute() = 0; // methode pure virtuelle chaque classe dérivée doit l'implémenter

protected:

    // Getters
    int getId() const;
    string getType() const;
};

#endif // indique la fin de la zone protégée