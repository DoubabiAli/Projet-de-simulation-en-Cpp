#ifndef SOUND_SENSOR_H
#define SOUND_SENSOR_H

#include "Sensor.h"

class SoundSensor : public Sensor {
private:
    int valeur;

public:
    SoundSensor(Server* server);

    void execute() override;
    int getValeur() const;
};

#endif