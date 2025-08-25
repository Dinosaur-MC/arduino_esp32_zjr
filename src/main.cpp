#include <Arduino.h>

#include "button.h"
#include "event.h"
#include "labview_api.h"
#include "light_effects.h"
#include "sensor.h"

using namespace events;

// 单场比赛时间（单位：s）
#define COMPETITION_TIME_S 30

// 计时器间隔（单位：ms）
#define TIMER_INTERVAL_MS 1000
// 计时器
int32_t timer     = 0;
int32_t pre_timer = COMPETITION_TIME_S;

#define PIN_BTN_START 25  // 开始按钮引脚
#define PIN_BTN_PAUSE 21  // 暂停按钮引脚
#define PIN_BTN_RESET 17  // 重置按钮引脚
Button btn_start;         // 开始按钮
Button btn_pause;         // 暂停按钮
Button btn_reset;         // 重置按钮

// 玩家胜场
int32_t player_a_win = 0;
int32_t player_b_win = 0;
// 玩家本场得分
int32_t player_a_score = 0;
int32_t player_b_score = 0;

// 玩家引脚
#define PIN_PLAYER_A 5   // 玩家A传感器引脚
#define PIN_PLAYER_B 23  // 玩家B传感器引脚
// 玩家传感器
Sensor player_a;
Sensor player_b;

// 比赛场数
int32_t competition_count = 0;

// 比赛状态
enum class State : int32_t {
    Init = 0,
    Ready,
    Started,
    Paused,
    Ended,
};
State competition_state     = State::Init;
State pre_competition_state = State::Init;

// LED灯带引脚
#define PIN_LED 19
// LED灯带
SmartLed leds(LED_WS2812B, 7, PIN_LED, 0);

// DEBUG按钮引脚
#define PIN_DEBUG_BTN 33
Button debug_btn;
bool debug_mode    = false;
uint8_t debug_flag = 0;

namespace handler {

// 计时器事件处理
void timer_tick(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data) {
    labview::send_i32(labview::Tick, timer);
    if (competition_state == State::Started) {
        le::progress(leds, Rgb{0, 200, 0}, timer, COMPETITION_TIME_S, true);
    }
    DEBUGSERIAL.printf("Timer countdown: %d\n", timer);
}
void timer_timeout(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data) {
    if (competition_state == State::Ready) {
        event::post(EVENT_COMPETITION_START);
    } else if (competition_state == State::Started) {
        event::post(EVENT_COMPETITION_STOP);
    }
}

// 玩家事件处理
void player(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data) {
    labview::send_i32(labview::PlayerAScore, player_a_score);
    labview::send_i32(labview::PlayerBScore, player_b_score);
    int8_t p = event_data != NULL ? *(int8_t *)event_data : 0x00;
    if (event_id == EVENT_PLAYER_SCORE) {
        if (p == 0x01) {
            G_APPLY_LIGHT_EFFECT(
                le::progress(leds, Rgb{200, 100, 0}, timer, COMPETITION_TIME_S, true); delay(100);
                le::progress(leds, Rgb{0, 200, 0}, timer, COMPETITION_TIME_S, true)
            );
            DEBUGSERIAL.println("Player A +1 score");
        } else if (p == 0x02) {
            G_APPLY_LIGHT_EFFECT(
                le::progress(leds, Rgb{0, 100, 200}, timer, COMPETITION_TIME_S, true); delay(100);
                le::progress(leds, Rgb{0, 200, 0}, timer, COMPETITION_TIME_S, true)
            );
            DEBUGSERIAL.println("Player B +1 score");
        }
    } else if (event_id == EVENT_PLAYER_WIN) {
        if (p == 0x01) {
            G_APPLY_LIGHT_EFFECT_INF(
                le::right_spread(leds, Rgb{255, 0, 0}, 1000), 1000,
                competition_state != State::Ready && competition_state != State::Init
            );
            DEBUGSERIAL.println("Player A win");
        } else if (p == 0x02) {
            G_APPLY_LIGHT_EFFECT_INF(
                le::left_spread(leds, Rgb{0, 0, 255}, 1000), 1000,
                competition_state != State::Ready && competition_state != State::Init
            );
            DEBUGSERIAL.println("Player B win");
        }
    } else if (event_id == EVENT_PLAYER_DRAW) {
        G_APPLY_LIGHT_EFFECT_INF(
            le::double_spread(leds, Rgb{255, 0, 0}, Rgb{0, 0, 255}, 1000, true), 1000,
            competition_state != State::Ready && competition_state != State::Init
        );
        DEBUGSERIAL.println("Draw");
    }
}

// 玩家传感器信号处理
void sensor_player_a(int32_t value, int8_t state) {
    if (state == Button::FALL) {
        player_a_score++;
        int8_t data = 0x01;
        event::post(EVENT_PLAYER_SCORE, &data, sizeof(data));
    }
}
void sensor_player_b(int32_t value, int8_t state) {
    if (state == Button::FALL) {
        player_b_score++;
        int8_t data = 0x02;
        event::post(EVENT_PLAYER_SCORE, &data, sizeof(data));
    }
}

// 比赛事件处理
void competition(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data) {
    switch (event_id) {
        case EVENT_COMPETITION_INITED: {
            DEBUGSERIAL.println("Competition inited");
        } break;
        case EVENT_COMPETITION_READY: {
            competition_state = State::Ready;
            timer             = 3 * 1000 / TIMER_INTERVAL_MS;
            player_a_score    = 0;
            player_b_score    = 0;
            G_APPLY_LIGHT_EFFECT(le::middle_spread(leds, Rgb{200, 200, 0}, 3000));
            DEBUGSERIAL.println("Competition ready");
        } break;
        case EVENT_COMPETITION_START: {
            competition_state = State::Started;
            timer             = pre_timer * 1000 / TIMER_INTERVAL_MS;
            player_a.begin(PIN_PLAYER_A, true, handler::sensor_player_a, 100);
            player_b.begin(PIN_PLAYER_B, true, handler::sensor_player_b, 100);
            le::clear(leds);
            DEBUGSERIAL.println("Competition started");
        } break;
        case EVENT_COMPETITION_PAUSE: {
            player_a.end();
            player_b.end();
            competition_state = State::Paused;
            pre_timer         = timer;
            G_APPLY_LIGHT_EFFECT_INF(
                le::breath(leds, Rgb{200, 200, 0}, 16, 0.5), 200, competition_state == State::Paused
            );
            DEBUGSERIAL.println("Competition paused");
        } break;
        case EVENT_COMPETITION_RESUME: {
            competition_state = State::Ready;
            timer             = 3 * 1000 / TIMER_INTERVAL_MS;
            G_APPLY_LIGHT_EFFECT(le::middle_spread(leds, Rgb{200, 200, 0}, 3000));
            DEBUGSERIAL.println("Competition resumed");
        } break;
        case EVENT_COMPETITION_STOP: {
            competition_state = State::Ended;
            player_a.end();
            player_b.end();
            pre_timer = COMPETITION_TIME_S;

            if (timer == 0) {
                competition_count++;
                if (player_a_score > player_b_score) {
                    player_a_win++;
                    int8_t data = 0x01;
                    event::post(EVENT_PLAYER_WIN, &data, sizeof(data));
                } else if (player_a_score < player_b_score) {
                    player_b_win++;
                    int8_t data = 0x02;
                    event::post(EVENT_PLAYER_WIN, &data, sizeof(data));
                } else {
                    event::post(EVENT_PLAYER_DRAW);
                }
            }
            labview::send_i32(labview::PlayerAWin, player_a_win);
            labview::send_i32(labview::PlayerBWin, player_b_win);
            labview::send_i32(labview::CompetitionCount, competition_count);

            DEBUGSERIAL.println("Competition stopped");
            if (timer == 0) {
                DEBUGSERIAL.printf(
                    "Player A: %d - %d\nPlayer B: %d - %d\n", player_a_score, player_a_win, player_b_score,
                    player_b_win
                );
                DEBUGSERIAL.printf("Competition count: %d\n", competition_count);
            } else {
                le::clear(leds);
            }

            timer = 0;
            event::post(EVENT_COMPETITION_INITED);
        } break;
        case EVENT_COMPETITION_RESET: {
            player_a.end();
            player_b.end();
            timer             = 0;
            pre_timer         = COMPETITION_TIME_S;
            competition_state = State::Ended;
            competition_count = 0;
            player_a_win      = 0;
            player_b_win      = 0;
            player_a_score    = 0;
            player_b_score    = 0;
            labview::send_i32(labview::Tick, timer);
            labview::send_i32(labview::CompetitionCount, competition_count);
            labview::send_i32(labview::CompetitionState, (int32_t)competition_state);
            labview::send_i32(labview::PlayerAWin, player_a_win);
            labview::send_i32(labview::PlayerBWin, player_b_win);
            labview::send_i32(labview::PlayerAScore, player_a_score);
            labview::send_i32(labview::PlayerBScore, player_b_score);
            event::post(EVENT_COMPETITION_INITED);
            le::light(leds, Rgb{255, 0, 0}, Rgb{0, 0, 255}, true);
            DEBUGSERIAL.println("Competition reset");
        } break;
        default: {
            DEBUGSERIAL.print("Unknown event: ");
            DEBUGSERIAL.println(event_id);
        } break;
    }
    if (pre_competition_state != competition_state) {
        labview::send_i32(labview::CompetitionState, (int32_t)competition_state);
        pre_competition_state = competition_state;
    }
}

// 控制信号处理
void button_satrt(int32_t value, int8_t state) {
    if (state == Button::FALL) {
        if (competition_state == State::Init) {
            event::post(EVENT_COMPETITION_RESET);
            event::post(EVENT_COMPETITION_READY);
        } else if (competition_state == State::Ended) {
            event::post(EVENT_COMPETITION_READY);
        } else {
            event::post(EVENT_COMPETITION_STOP);
        }
    }
}
void button_pause(int32_t value, int8_t state) {
    if (state == Button::FALL) {
        if (competition_state == State::Started) {
            event::post(EVENT_COMPETITION_PAUSE);
        } else if (competition_state == State::Paused) {
            event::post(EVENT_COMPETITION_RESUME);
        }
    }
}
void button_reset(int32_t value, int8_t state) {
    if (state == Button::FALL) {
        if (competition_state == State::Init || competition_state == State::Ended) {
            event::post(EVENT_COMPETITION_RESET);
        }
    }
}

}  // namespace handler

// 程序入口
void setup() {
    // 初始化串口
    DEBUGSERIAL.begin(115200);
    DEBUGSERIAL.println("Initializing...");

    debug_btn.begin(PIN_DEBUG_BTN, true, [](int32_t value, int8_t state) {
        if (state == Button::FALL) {
            debug_mode = !debug_mode;
            if (debug_mode) {
                xTaskCreate(
                    [](void *) {
                        while (debug_mode) {
                            LABVIEWSERIAL.write((char *)&debug_flag, sizeof(debug_flag));
                            delay(1000);
                            LABVIEWSERIAL.println(debug_flag);
                            delay(1000);
                            debug_flag++;
                        }
                        vTaskDelete(NULL);
                    },
                    "debug_task", 2048, NULL, 1, NULL
                );
            }
        }
    });

    // 初始化事件系统
    event::init();
    event::post(ZJR_EVENT_BASE, EVENT_LOOP_TEST_EVENT, NULL, 0);

    // 初始化LabView API
    labview::init();

    // 注册事件处理程序
    event::register_handlers(EVENT_TIMER_TICK, handler::timer_tick, NULL);
    event::register_handlers(EVENT_TIMER_TIMEOUT, handler::timer_timeout, NULL);
    event::register_handlers(EVENT_PLAYER_SCORE, handler::player, NULL);
    event::register_handlers(EVENT_PLAYER_WIN, handler::player, NULL);
    event::register_handlers(EVENT_PLAYER_DRAW, handler::player, NULL);
    event::register_handlers(EVENT_COMPETITION_INITED, handler::competition, NULL);
    event::register_handlers(EVENT_COMPETITION_READY, handler::competition, NULL);
    event::register_handlers(EVENT_COMPETITION_START, handler::competition, NULL);
    event::register_handlers(EVENT_COMPETITION_PAUSE, handler::competition, NULL);
    event::register_handlers(EVENT_COMPETITION_RESUME, handler::competition, NULL);
    event::register_handlers(EVENT_COMPETITION_STOP, handler::competition, NULL);
    event::register_handlers(EVENT_COMPETITION_RESET, handler::competition, NULL);
    event::register_handlers(
        EVENT_LABVIEW_BTN_START,
        [](void *, esp_event_base_t, int32_t, void *) { handler::button_satrt(0, Button::FALL); }, NULL
    );
    event::register_handlers(
        EVENT_LABVIEW_BTN_PAUSE,
        [](void *, esp_event_base_t, int32_t, void *) { handler::button_pause(0, Button::FALL); }, NULL
    );
    event::register_handlers(
        EVENT_LABVIEW_BTN_RESET,
        [](void *, esp_event_base_t, int32_t, void *) { handler::button_reset(0, Button::FALL); }, NULL
    );

    // 初始化比赛控制按钮
    btn_start.begin(PIN_BTN_START, true, handler::button_satrt, 100);
    btn_pause.begin(PIN_BTN_PAUSE, true, handler::button_pause, 100);
    btn_reset.begin(PIN_BTN_RESET, true, handler::button_reset, 100);

    // 初始化LED灯
    le::clear(leds);
    DEBUGSERIAL.println("Done!");
}

// 定时器循环
void loop() {
    if (timer > 0 && competition_state != State::Paused && competition_state != State::Ended) {
        if (timer > 0) {
            timer--;
            event::post(EVENT_TIMER_TICK, &timer, sizeof(timer));
            if (timer == 0) event::post(EVENT_TIMER_TIMEOUT, NULL, 0);
        }
        delay(TIMER_INTERVAL_MS);
    }
}
