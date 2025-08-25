#pragma once

#include <SmartLeds.h>

#define APPLY_LIGHT_EFFECT(leds, le_func, ...)          \
    do {                                                \
        xTaskCreate(                                    \
            [](void *arg) {                             \
                le_func(*(SmartLed *)arg, __VA_ARGS__); \
                vTaskDelete(NULL);                      \
            },                                          \
            "LE", 2048, &(leds), 1, NULL                \
        );                                              \
    } while (false)

#define G_APPLY_LIGHT_EFFECT(le_func) \
    do {                              \
        xTaskCreate(                  \
            [](void *) {              \
                le_func;              \
                vTaskDelete(NULL);    \
            },                        \
            "LE", 2048, NULL, 1, NULL \
        );                            \
    } while (false)

#define APPLY_LIGHT_EFFECT_NUM(leds, num, interval, le_func, ...) \
    do {                                                          \
        xTaskCreate(                                              \
            [](void *arg) {                                       \
                int32_t n = (num);                                \
                while (n--) {                                     \
                    le_func(*(SmartLed *)arg, __VA_ARGS__);       \
                    vTaskDelay((interval) / portTICK_PERIOD_MS);  \
                }                                                 \
                vTaskDelete(NULL);                                \
            },                                                    \
            "LE", 2048, &(leds), 1, NULL                          \
        );                                                        \
    } while (false)

#define G_APPLY_LIGHT_EFFECT_NUM(le_func, num, interval)         \
    do {                                                         \
        xTaskCreate(                                             \
            [](void *) {                                         \
                int32_t n = (num);                               \
                while (n--) {                                    \
                    le_func;                                     \
                    vTaskDelay((interval) / portTICK_PERIOD_MS); \
                }                                                \
                vTaskDelete(NULL);                               \
            },                                                   \
            "LE", 2048, NULL, 1, NULL                            \
        );                                                       \
    } while (false)

#define G_APPLY_LIGHT_EFFECT_INF(le_func, interval, signal_)     \
    do {                                                         \
        xTaskCreate(                                             \
            [](void *) {                                         \
                while (signal_) {                                \
                    le_func;                                     \
                    vTaskDelay((interval) / portTICK_PERIOD_MS); \
                }                                                \
                vTaskDelete(NULL);                               \
            },                                                   \
            "LE", 2048, NULL, 1, NULL                            \
        );                                                       \
    } while (false)

// Light effects
namespace le {

// 清除
void clear(SmartLed &leds);

// 纯色常亮
void light(SmartLed &leds, const Rgb &color);

// 纯色常亮（范围）
void light(SmartLed &leds, const Rgb &color, uint32_t begin, uint32_t end, bool fill_blank = false);

// 双色常亮
void light(SmartLed &leds, const Rgb &color1, const Rgb &color2, bool mix = false);

// 左流水灯
void left_stream(SmartLed &leds, const Hsv &color, int32_t length = 1, uint8_t t = 100);

// 右流水灯
void right_stream(SmartLed &leds, const Hsv &color, int32_t length = 1, uint8_t t = 100);

// 纯色闪烁
void flash(SmartLed &leds, const Hsv &color, uint8_t flash_time = 200);

// 双色闪烁
void flash(SmartLed &leds, const Rgb &color1, const Rgb &color2, uint8_t flash_time = 200);

// 双色闪烁（范围）
void flash(
    SmartLed &leds, uint32_t begin, uint32_t end, const Rgb &color1, const Rgb &color2,
    uint8_t flash_time = 200
);

// 单色呼吸
void breath(SmartLed &leds, const Hsv &color, uint32_t scale = 32, double speed = 0.5, int16_t step = 127);

// 中间扩散
void middle_spread(SmartLed &leds, const Rgb &color, uint32_t spread_time);

// 左侧扩散
void left_spread(SmartLed &leds, const Rgb &color, uint32_t spread_time);

// 右侧扩散
void right_spread(SmartLed &leds, const Rgb &color, uint32_t spread_time);

// 两侧单色扩散
void double_spread(SmartLed &leds, const Rgb &color, uint32_t spread_time);

// 两侧双色扩散
void double_spread(
    SmartLed &leds, const Rgb &color1, const Rgb &color2, uint32_t spread_time, bool mix = false
);

// 进度条
void progress(
    SmartLed &leds, const Rgb &color, uint32_t progress, uint32_t max_progress, bool reverse = false
);

}  // namespace le
