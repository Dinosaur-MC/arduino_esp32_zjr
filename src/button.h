#pragma once
#include <esp_task.h>

#include <cinttypes>

#include "event.h"

ESP_EVENT_DECLARE_BASE(BUTTON_EVENT_BASE);

// 按钮
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

    void begin(uint8_t pin, bool pullup = true);  // 手动管理模式，无事件
    void begin(
        uint8_t pin, bool pullup, void (*callback)(int32_t, int8_t), uint32_t interval_ms = 10
    );               // 事件回调模式
    void end();      // 停止
    int32_t read();  // 读取按钮状态

   protected:
    static TaskFunction_t _taskfunc;

    uint8_t _pin;
    uint32_t _interval_ms;
    int32_t _value;
    int32_t _pre_value;

    TaskHandle_t _task_handle;
    void (*_callback)(int32_t value, int8_t state);
};
