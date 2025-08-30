#pragma once
#include <esp_task.h>

#include "button.h"

ESP_EVENT_DECLARE_BASE(SENSOR_EVENT_BASE);

/**
 * @brief 接触传感器
 * 相当于按钮，可增加滤波功能（或在电路中添加合适的滤波电容）
 * 这个本来是为压力传感器准备的，但是因为没用到就删了相关逻辑
 */
class Sensor : public Button {
   public:
    Sensor();
    ~Sensor();

   private:
};
