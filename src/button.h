#pragma once
#include <esp_task.h>

#include <cinttypes>

#include "event.h"

ESP_EVENT_DECLARE_BASE(BUTTON_EVENT_BASE);

// 开关传感器
class Button {
   public:
    enum State {
        TOGGLE = -1,
        FALL   = 0,
        RAISE  = 1,
        IDLE   = 2,
    };

    Button();
    virtual ~Button();

    void begin(uint8_t pin, bool pullup = true);
    void begin(uint8_t pin, bool pullup, void (*callback)(int32_t, int8_t), uint32_t interval_ms = 10);
    void end();
    int32_t read();

   protected:
    static TaskFunction_t _taskfunc;

    uint8_t _pin;
    uint32_t _interval_ms;
    int32_t _value;
    int32_t _pre_value;

    TaskHandle_t _task_handle;
    void (*_callback)(int32_t value, int8_t state);
};
