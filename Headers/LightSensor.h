#ifndef LIGHT_SENSOR_H
#define LIGHT_SENSOR_H

#include "Sensor.h"

class LightSensor : public Sensor {
public:
    LightSensor(Server* server);

    void execute() override;
};

#endif
    