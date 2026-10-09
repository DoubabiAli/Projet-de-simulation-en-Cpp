#ifndef TEMPERATURE_SENSOR_H
#define TEMPERATURE_SENSOR_H

#include "Sensor.h"

class TemperatureSensor : public Sensor {
public:
    TemperatureSensor(Server* server);

    void execute() override;
};

#endif