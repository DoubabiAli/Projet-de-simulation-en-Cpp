#ifndef HUMIDITY_SENSOR_H
#define HUMIDITY_SENSOR_H

#include "Sensor.h"

class HumiditySensor : public Sensor {
private:
    float valeur;

public:
    HumiditySensor(Server* server);

    void execute() override;
    float getValeur() const;
};

#endif