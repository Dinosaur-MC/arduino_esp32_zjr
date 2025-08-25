#pragma once
#include <esp_task.h>

#include "button.h"

ESP_EVENT_DECLARE_BASE(SENSOR_EVENT_BASE);

// 开关传感器
class Sensor : public Button {
   public:
    Sensor();
    ~Sensor();

   private:
};
