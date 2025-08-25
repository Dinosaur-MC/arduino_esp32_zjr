#include "sensor.h"

#include <Arduino.h>

#include "events.h"

ESP_EVENT_DEFINE_BASE(SENSOR_EVENT_BASE);

Sensor::Sensor() : Button() {}
Sensor::~Sensor() {}
