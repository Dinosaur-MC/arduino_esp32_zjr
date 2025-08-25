#include "button.h"

#include <Arduino.h>

#include "events.h"

using namespace events;

ESP_EVENT_DEFINE_BASE(BUTTON_EVENT_BASE);

Button::Button() {
    _value       = 0;
    _pre_value   = 0;
    _task_handle = NULL;
    _callback    = NULL;
    _interval_ms = 10;
}
Button::~Button() { end(); }

void Button::begin(uint8_t pin, bool pullup) {
    _pin = pin;
    pinMode(pin, pullup ? INPUT_PULLUP : INPUT);
}
void Button::begin(uint8_t pin, bool pullup, void (*callback)(int32_t, int8_t), uint32_t interval_ms) {
    begin(pin, pullup);
    if (_task_handle == NULL && _callback == NULL) {
        _callback    = callback;
        _interval_ms = interval_ms;
        xTaskCreate(_taskfunc, "btn_task", 2048, this, 5, &_task_handle);
    }
}

void Button::end() {
    if (_task_handle != NULL) {
        vTaskDelete(_task_handle);
        _task_handle = NULL;
    }
    if (_callback != NULL) {
        _callback = NULL;
    }
}

int32_t Button::read() { return digitalRead(_pin); }

TaskFunction_t Button::_taskfunc = [](void *arg) {
    Button *btn     = (Button *)arg;
    btn->_pre_value = btn->read();
    while (true) {
        btn->_value = btn->read();
        if (btn->_value > btn->_pre_value) {
            (btn->_callback)(btn->_value, RAISE);
        } else if (btn->_value < btn->_pre_value) {
            (btn->_callback)(btn->_value, FALL);
        }
        if (btn->_value != btn->_pre_value) {
            (btn->_callback)(btn->_value, TOGGLE);
            btn->_pre_value = btn->_value;
        }
        vTaskDelay(btn->_interval_ms / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
};
