#ifndef TEMPERATURE_SENSOR_H
#define TEMPERATURE_SENSOR_H

#include "Sensor.h"

class TemperatureSensor : public Sensor {
private:
    float valeur;

public:
    TemperatureSensor(Server* server);

    void execute() override;
    float getValeur() const;
};

#endif