#ifndef HUMIDITY_SENSOR_H
#define HUMIDITY_SENSOR_H

#include "Sensor.h"

class HumiditySensor : public Sensor {
public:
    HumiditySensor(Server* server);

    void execute() override;
};

#endif