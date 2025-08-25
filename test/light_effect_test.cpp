
#include <Arduino.h>
#include <SmartLeds.h>

#include "button.h"
#include "light_effects.h"

// Debug按钮
Button debug_button;
Button debug_button2;
// Debug状态
uint32_t debug = 0;

const uint8_t PIN_LED = 19;
SmartLed leds(LED_WS2812B, 7, PIN_LED, 0, SingleBuffer);

void light_effect_test() {
    auto dbg_callback = [](int32_t value, int8_t state) {
        if (state == Button::FALL) {
            DEBUGSERIAL.printf("[Debug] %d\n", debug);
            le::clear(leds);
            delay(100);
            switch (debug++) {
                case 0:
                    DEBUGSERIAL.println("Clear");
                    break;
                case 1:
                    APPLY_LIGHT_EFFECT(leds, le::light, Rgb{255, 255, 0});
                    // le::light(leds, Rgb{255, 255, 0});
                    DEBUGSERIAL.println("1:0");
                    delay(1000);
                    le::light(leds, Rgb{220, 220, 0});
                    DEBUGSERIAL.println("1:1");
                    delay(1000);
                    le::light(leds, Rgb{255, 0, 0}, Rgb{0, 0, 255});
                    DEBUGSERIAL.println("1:2");
                    delay(1000);
                    DEBUGSERIAL.println("1:3");
                    le::light(leds, Rgb{220, 0, 0}, Rgb{0, 0, 220}, true);
                    DEBUGSERIAL.println("1:4");
                    delay(1000);
                    break;
                case 2:
                    le::light(leds, Rgb{255, 0, 0}, 0, 3, true);
                    DEBUGSERIAL.println("2:0");
                    delay(500);
                    le::light(leds, Rgb{0, 255, 0}, 3, 4);
                    DEBUGSERIAL.println("2:1");
                    delay(500);
                    le::light(leds, Rgb{0, 0, 255}, 4, 7);
                    DEBUGSERIAL.println("2:2");
                    delay(500);
                    break;
                case 3:
                    le::right_stream(leds, Rgb{255, 255, 0}, 1);
                    DEBUGSERIAL.println("3:0");
                    delay(500);
                    le::left_stream(leds, Rgb{255, 255, 0}, 1);
                    DEBUGSERIAL.println("3:1");
                    delay(500);
                    le::right_stream(leds, Rgb{255, 255, 0}, 2);
                    DEBUGSERIAL.println("3:2");
                    delay(500);
                    le::left_stream(leds, Rgb{255, 255, 0}, 2);
                    DEBUGSERIAL.println("3:3");
                    delay(500);
                    le::right_stream(leds, Rgb{255, 255, 0}, 5);
                    DEBUGSERIAL.println("3:4");
                    delay(500);
                    le::left_stream(leds, Rgb{255, 255, 0}, 5);
                    DEBUGSERIAL.println("3:5");
                    delay(500);
                    break;
                case 4:
                    le::flash(leds, (Hsv)Rgb{220, 0, 0}, 200);
                    DEBUGSERIAL.println("4:0");
                    delay(500);
                    le::flash(leds, Rgb{0, 220, 0}, Rgb{0, 0, 220}, 200);
                    DEBUGSERIAL.println("4:1");
                    delay(500);
                    le::flash(leds, 2, 5, Rgb{0, 220, 0}, Rgb{220, 0, 220}, 200);
                    DEBUGSERIAL.println("4:2");
                    delay(500);
                    break;
                case 5:
                    le::breath(leds, Rgb{220, 220, 0}, 32, 0.5);
                    DEBUGSERIAL.println("5:0");
                    delay(500);
                    le::breath(leds, Rgb{255, 255, 0}, 32, 1);
                    DEBUGSERIAL.println("5:1");
                    delay(500);
                    le::breath(leds, Rgb{255, 255, 0}, 12, 0.5);
                    DEBUGSERIAL.println("5:2");
                    delay(500);
                    break;
                case 6:
                    le::middle_spread(leds, Rgb{255, 255, 0}, 1000);
                    DEBUGSERIAL.println("6:0");
                    delay(500);
                    le::middle_spread(leds, Rgb{200, 200, 0}, 3000);
                    DEBUGSERIAL.println("6:1");
                    delay(500);
                    break;
                case 7:
                    le::right_spread(leds, Rgb{255, 0, 0}, 1000);
                    DEBUGSERIAL.println("7:0");
                    delay(500);
                    le::left_spread(leds, Rgb{0, 0, 255}, 1000);
                    DEBUGSERIAL.println("7:1");
                    delay(500);
                    break;
                case 8:
                    le::double_spread(leds, Rgb{0, 255, 0}, 1000);
                    DEBUGSERIAL.println("8:0");
                    delay(500);
                    le::double_spread(leds, Rgb{255, 0, 0}, Rgb{0, 0, 255}, 1000);
                    DEBUGSERIAL.println("8:1");
                    delay(500);
                    le::double_spread(leds, Rgb{255, 0, 0}, Rgb{0, 0, 255}, 1000, true);
                    DEBUGSERIAL.println("8:2");
                    delay(500);
                    break;

                case 9:
                    le::progress(leds, Rgb{0, 255, 0}, 0, 30);
                    DEBUGSERIAL.println("9:0");
                    delay(200);
                    le::progress(leds, Rgb{0, 255, 0}, 5, 30);
                    DEBUGSERIAL.println("9:1");
                    delay(200);
                    le::progress(leds, Rgb{0, 255, 0}, 15, 30);
                    DEBUGSERIAL.println("9:2");
                    delay(200);
                    le::progress(leds, Rgb{0, 255, 0}, 20, 30);
                    DEBUGSERIAL.println("9:3");
                    delay(200);
                    le::progress(leds, Rgb{0, 255, 0}, 25, 30);
                    DEBUGSERIAL.println("9:4");
                    delay(200);
                    le::progress(leds, Rgb{0, 255, 0}, 30, 30);
                    DEBUGSERIAL.println("9:5");
                    delay(200);

                    le::progress(leds, Rgb{200, 200, 200}, 0, 30, true);
                    DEBUGSERIAL.println("9:6");
                    delay(200);
                    le::progress(leds, Rgb{200, 200, 200}, 5, 30, true);
                    DEBUGSERIAL.println("9:7");
                    delay(200);
                    le::progress(leds, Rgb{200, 200, 200}, 15, 30, true);
                    DEBUGSERIAL.println("9:8");
                    delay(200);
                    le::progress(leds, Rgb{200, 200, 200}, 20, 30, true);
                    DEBUGSERIAL.println("9:9");
                    delay(200);
                    le::progress(leds, Rgb{200, 200, 200}, 25, 30, true);
                    DEBUGSERIAL.println("9:10");
                    delay(200);
                    le::progress(leds, Rgb{200, 200, 200}, 30, 30, true);
                    DEBUGSERIAL.println("9:11");
                    delay(200);
                    break;
                default:
                    debug = 0;
                    break;
            }
        }
    };

    debug_button.begin(18, true, dbg_callback);
    debug_button2.begin(5, true, [](int32_t value, int8_t state) {
        if (state == Button::FALL) {
            DEBUGSERIAL.printf("[Debug] -> %d\n", debug > 0 ? --debug : debug);
        }
    });
}
