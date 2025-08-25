#pragma once

#include <Arduino.h>

#define LABVIEWSERIAL Serial1
#define LABVIEWSERIAL_BAUD 115200
#define PIN_LABVIEW_RX 22
#define PIN_LABVIEW_TX 26

namespace labview {

enum Cmd : uint8_t {
    Init = 0x01,       // 初始化
    Tick,              // 倒计时 i32
    Timeout,           // 超时
    Reset,             // 重置
    Ready,             // 准备
    Start,             // 开始
    Pause,             // 暂停
    Resume,            // 恢复
    Stop = 0x09,       // 停止
    PlayerAScore,      // 玩家A得分 i32
    PlayerBScore,      // 玩家B得分 i32
    PlayerAWin,        // 玩家A胜利次数 i32
    PlayerBWin,        // 玩家B胜利次数 i32
    Draw,              // 平局
    CompetitionCount,  // 比赛次数 i32
    CompetitionState,  // 比赛状态 i32
    R_BTN_START,       // 开始按键
    R_BTN_PAUSE,       // 暂停按键
    R_BTN_RESET,       // 重置按键
};

void init();
void deinit();

void send_command(uint8_t cmd, uint8_t *data = nullptr, uint8_t size = 0);
void send_i32(uint8_t address, int32_t value);
void send_f32(uint8_t address, double value);

}  // namespace labview
