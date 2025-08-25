#pragma once
#include <esp_event.h>

#include "events.h"

#define DEBUGSERIAL Serial

// 声明事件基
ESP_EVENT_DECLARE_BASE(ZJR_EVENT_BASE);

namespace event {

// 初始化事件系统
void init();

// 销毁事件系统
void deinit();

// 注册事件处理函数
void register_handlers(
    esp_event_base_t event_base, events::Event event_id, esp_event_handler_t handler, void *arg
);
void register_handlers(events::Event event_id, esp_event_handler_t handler, void *arg);

// 注销事件处理函数
void unregister_handlers(esp_event_base_t event_base, events::Event event_id, esp_event_handler_t handler);
void unregister_handlers(events::Event event_id, esp_event_handler_t handler);

// 发送事件
void post(esp_event_base_t event_base, events::Event event_id, void *data, size_t size);
void post(events::Event event_id, void *data = NULL, size_t size = 0);

}  // namespace event
