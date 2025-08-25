#include "light_effects.h"

#include <Arduino.h>

#include <cmath>

namespace le {

// 清除
void clear(SmartLed &leds) { light(leds, Rgb{0, 0, 0}); }

// 纯色常亮
void light(SmartLed &leds, const Rgb &color) {
    for (uint32_t i = 0; i < leds.size(); i++) leds[i] = color;
    leds.wait();
    leds.show();
}

// 纯色常亮（范围）
void light(SmartLed &leds, const Rgb &color, uint32_t begin, uint32_t end, bool fill_blank) {
    if (fill_blank)
        for (uint32_t i = 0; i < leds.size(); i++) leds[i] = Rgb{0, 0, 0};
    if (begin >= end || begin >= leds.size() || end <= 0) return;
    if (end > leds.size()) end = leds.size();
    if (begin < 0) begin = 0;

    for (uint32_t i = begin; i < end; i++) leds[i] = color;
    leds.wait();
    leds.show();
}

// 双色常亮
void light(SmartLed &leds, const Rgb &color1, const Rgb &color2, bool mix) {
    int32_t mid = leds.size() / 2;
    for (int32_t i = 0; i < leds.size(); i++) {
        if (i < mid)
            leds[i] = color1;
        else
            leds[i] = color2;
    }
    if (mix && leds.size() % 2 == 1) {
        Rgb hc1   = Rgb{uint8_t(color1.r / 2), uint8_t(color1.g / 2), uint8_t(color1.b / 2)};
        Rgb hc2   = Rgb{uint8_t(color2.r / 2), uint8_t(color2.g / 2), uint8_t(color2.b / 2)};
        leds[mid] = hc1 + hc2;
    }
    leds.wait();
    leds.show();
}

// 左流水灯
void left_stream(SmartLed &leds, const Hsv &color, int32_t length, uint8_t t) {
    for (int32_t i = 0; i < leds.size(); i++) leds[i] = Rgb{0, 0, 0};
    for (int32_t i = leds.size() - 1; i >= -length; i--) {
        if (i + length < leds.size()) leds[i + length] = Rgb{0, 0, 0};
        for (int32_t j = 0; j < length; j++) {
            if (i + j >= 0 && i + j < leds.size()) {
                leds[i + j] = color;
            }
        }
        leds.wait();
        leds.show();
        if (i > -length) delay(t);
    }
}

// 右流水灯
void right_stream(SmartLed &leds, const Hsv &color, int32_t length, uint8_t t) {
    for (int32_t i = 0; i < leds.size(); i++) leds[i] = Rgb{0, 0, 0};
    for (int32_t i = 1 - length; i <= leds.size(); i++) {
        if (i - 1 >= 0) leds[i - 1] = Rgb{0, 0, 0};
        for (int32_t j = 0; j < length; j++) {
            if (0 <= i + j && i + j < leds.size()) {
                leds[i + j] = color;
            }
        }
        leds.wait();
        leds.show();
        if (i < leds.size()) delay(t);
    }
}

// 纯色闪烁
void flash(SmartLed &leds, const Hsv &color, uint8_t flash_time) {
    for (int32_t i = 0; i < leds.size(); i++) leds[i] = Hsv{color.h, (uint8_t)(color.s * 0.75), 255};
    leds.wait();
    leds.show();
    delay(flash_time);

    for (int32_t i = 0; i < leds.size(); i++) leds[i] = color;
    leds.wait();
    leds.show();
}

// 双色闪烁
void flash(SmartLed &leds, const Rgb &color1, const Rgb &color2, uint8_t flash_time) {
    for (int32_t i = 0; i < leds.size(); i++) leds[i] = color1;
    leds.wait();
    leds.show();
    delay(flash_time);

    for (int32_t i = 0; i < leds.size(); i++) leds[i] = color2;
    leds.wait();
    leds.show();
}

// 双色闪烁（范围）
void flash(
    SmartLed &leds, uint32_t begin, uint32_t end, const Rgb &color1, const Rgb &color2, uint8_t flash_time
) {
    if (begin >= end || begin >= leds.size() || end <= 0) return;
    if (end > leds.size()) end = leds.size();
    if (begin < 0) begin = 0;

    for (int32_t i = begin; i < end; i++) leds[i] = color1;
    leds.wait();
    leds.show();
    delay(flash_time);

    for (int32_t i = begin; i < end; i++) leds[i] = color2;
    leds.wait();
    leds.show();
}

// 单色呼吸
void breath(SmartLed &leds, const Hsv &color, uint32_t scale, double speed, int16_t step) {
    for (int32_t i = 0; i < step / 2; i++) {
        double v = 255.0 * i * 2 / step;
        for (int32_t j = 0; j < leds.size(); j++) {
            leds[j] = Hsv{color.h, color.s, (uint8_t)v};
        }
        leds.wait();
        leds.show();
        delay(scale * pow(abs(cos(2 * PI * i / (double)step) + 1) / 2, speed) + 1);
    }
    for (int32_t i = step / 2; i <= step; i++) {
        double v = 255.0 * (2 * (step - i)) / step;
        for (int32_t j = 0; j < leds.size(); j++) {
            leds[j] = Hsv{color.h, color.s, (uint8_t)v};
        }
        leds.wait();
        leds.show();
        delay(scale * pow(abs(cos(2 * PI * i / (double)step) + 1) / 2, speed) + 1);
    }
}

// 中间扩散
void middle_spread(SmartLed &leds, const Rgb &color, uint32_t spread_time) {
    uint32_t t  = spread_time / (leds.size() - 1);
    int32_t mid = leds.size() / 2;
    for (int32_t i = 0; i <= mid; i++) {
        int32_t p = mid - i;
        for (int32_t j = 0; j < mid; j++) {
            if (j >= p)
                leds[j] = color;
            else
                leds[j] = Rgb{0, 0, 0};
        }
        for (int32_t j = mid; j < leds.size(); j++) {
            if (j < leds.size() - p)
                leds[j] = color;
            else
                leds[j] = Rgb{0, 0, 0};
        }
        leds.wait();
        leds.show();
        if (i < mid) delay(t);
    }
}

// 左侧扩散
void left_spread(SmartLed &leds, const Rgb &color, uint32_t spread_time) {
    uint32_t t = spread_time / (leds.size() - 1);
    for (int32_t i = leds.size() - 1; i >= 0; i--) {
        for (int32_t j = 0; j < leds.size(); j++) {
            if (j >= i)
                leds[j] = color;
            else
                leds[j] = Rgb{0, 0, 0};
        }
        leds.wait();
        leds.show();
        if (i > 0) delay(t);
    }
}

// 右侧扩散
void right_spread(SmartLed &leds, const Rgb &color, uint32_t spread_time) {
    uint32_t t = spread_time / (leds.size() - 1);
    for (int32_t i = 0; i < leds.size(); i++) {
        for (int32_t j = 0; j < leds.size(); j++) {
            if (j <= i)
                leds[j] = color;
            else
                leds[j] = Rgb{0, 0, 0};
        }
        leds.wait();
        leds.show();
        if (i < leds.size() - 1) delay(t);
    }
}

// 两侧单色扩散
void double_spread(SmartLed &leds, const Rgb &color, uint32_t spread_time) {
    uint32_t t  = spread_time / (leds.size() - 1);
    int32_t mid = leds.size() / 2;
    for (int32_t i = 0; i <= mid; i++) {
        for (int32_t j = 0; j < mid; j++) {
            if (j <= i)
                leds[j] = color;
            else
                leds[j] = Rgb{0, 0, 0};
        }
        for (int32_t j = mid; j < leds.size(); j++) {
            if (j >= leds.size() - 1 - i)
                leds[j] = color;
            else
                leds[j] = Rgb{0, 0, 0};
        }
        leds.wait();
        leds.show();
        if (i < mid) delay(t);
    }
}

// 两侧双色扩散
void double_spread(SmartLed &leds, const Rgb &color1, const Rgb &color2, uint32_t spread_time, bool mix) {
    uint32_t t  = spread_time / (leds.size() - 1);
    int32_t mid = leds.size() / 2;
    for (int32_t i = 0; i <= mid; i++) {
        for (int32_t j = 0; j < mid; j++) {
            if (j <= i)
                leds[j] = color1;
            else
                leds[j] = Rgb{0, 0, 0};
        }
        for (int32_t j = mid; j < leds.size(); j++) {
            if (j >= leds.size() - 1 - i)
                leds[j] = color2;
            else
                leds[j] = Rgb{0, 0, 0};
        }
        if (mix && i == mid && leds.size() % 2 == 1) {
            Rgb hc1   = Rgb{uint8_t(color1.r / 2), uint8_t(color1.g / 2), uint8_t(color1.b / 2)};
            Rgb hc2   = Rgb{uint8_t(color2.r / 2), uint8_t(color2.g / 2), uint8_t(color2.b / 2)};
            leds[mid] = hc1 + hc2;
        }
        leds.wait();
        leds.show();
        if (i < mid) delay(t);
    }
}

// 进度条
void progress(SmartLed &leds, const Rgb &color, uint32_t progress, uint32_t max_progress, bool reverse) {
    if (progress > max_progress) progress = max_progress;
    int32_t p = reverse ? leds.size() * (1.0 - progress / (double)max_progress)
                        : leds.size() * progress / (double)max_progress;
    for (int32_t i = 0; i < leds.size(); i++) leds[i] = Rgb{0, 0, 0};
    if (reverse)
        for (int32_t i = leds.size() - 1 - p; i >= 0; i--) leds[i] = color;
    else
        for (int32_t i = 0; i < p; i++) leds[i] = color;
    leds.wait();
    leds.show();
}

}  // namespace le
