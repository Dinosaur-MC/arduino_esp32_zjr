#include "event.h"

#include <Arduino.h>

using namespace events;

// 定义事件基
ESP_EVENT_DEFINE_BASE(ZJR_EVENT_BASE);

namespace event {

// 定义事件循环句柄
esp_event_loop_handle_t loop_handle;

// 定义事件系统参数
esp_event_loop_args_t loop_args = {
    .queue_size      = 16,
    .task_name       = "ZJR_EVENT",
    .task_priority   = 10,
    .task_stack_size = 4096,
    .task_core_id    = 1,
};

// 测试事件处理函数
void test_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data) {
    static int counter = 0;
    if (DEBUGSERIAL.availableForWrite())
        DEBUGSERIAL.printf("[ZJR_EVENT] %s: %d\tcounter: %d\n", event_base, event_id, counter++);
}

// 初始化事件系统
void init() {
    // 创建事件循环
    ESP_ERROR_CHECK(esp_event_loop_create(&loop_args, &loop_handle));
    ESP_ERROR_CHECK(esp_event_handler_register_with(
        loop_handle, ZJR_EVENT_BASE, EVENT_LOOP_TEST_EVENT, test_handler, NULL
    ));
    // 触发初始化事件
    ESP_ERROR_CHECK(
        esp_event_post_to(loop_handle, ZJR_EVENT_BASE, EVENT_LOOP_INITIALIZED, NULL, 0, portMAX_DELAY)
    );
}

// 销毁事件系统
void deinit() {
    ESP_ERROR_CHECK(
        esp_event_post_to(loop_handle, ZJR_EVENT_BASE, EVENT_LOOP_DEINITIALIZED, NULL, 0, portMAX_DELAY)
    );
    ESP_ERROR_CHECK(esp_event_loop_delete(loop_handle));
}

// 注册事件处理函数
void register_handlers(
    esp_event_base_t event_base, events::Event event_id, esp_event_handler_t handler, void *arg
) {
    ESP_ERROR_CHECK(esp_event_handler_register_with(loop_handle, event_base, event_id, handler, arg));
}
void register_handlers(events::Event event_id, esp_event_handler_t handler, void *arg) {
    ESP_ERROR_CHECK(esp_event_handler_register_with(loop_handle, ZJR_EVENT_BASE, event_id, handler, arg));
}

// 注销事件处理函数
void unregister_handlers(esp_event_base_t event_base, events::Event event_id, esp_event_handler_t handler) {
    ESP_ERROR_CHECK(esp_event_handler_unregister_with(loop_handle, event_base, event_id, handler));
}
void unregister_handlers(events::Event event_id, esp_event_handler_t handler) {
    ESP_ERROR_CHECK(esp_event_handler_unregister_with(loop_handle, ZJR_EVENT_BASE, event_id, handler));
}

// 发送事件
void post(esp_event_base_t event_base, events::Event event_id, void *data, size_t size) {
    ESP_ERROR_CHECK(esp_event_post_to(loop_handle, event_base, event_id, data, size, portMAX_DELAY));
}
void post(events::Event event_id, void *data, size_t size) {
    ESP_ERROR_CHECK(esp_event_post_to(loop_handle, ZJR_EVENT_BASE, event_id, data, size, portMAX_DELAY));
}

};  // namespace event
