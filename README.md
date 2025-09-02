# arduino_esp32_zjr - Arduino ESP32 竹节人大作战数字擂台

[![Release Version](https://img.shields.io/github/release/Dinosaur-MC/arduino_esp32_zjr.svg?style=plastic)](https://github.com/Dinosaur-MC/arduino_esp32_zjr/releases/latest)
[![LICENSE](https://img.shields.io/github/license/Dinosaur-MC/arduino_esp32_zjr)](LICENSE)
[![Bilibili Link](https://img.shields.io/badge/Bilibili-@他姓廖是宇宙的王-ff69b4.svg)](https://space.bilibili.com/1802840673)

## 介绍 | Introduction

本项目《竹节人大作战数字擂台》为一个竹节人对战记分系统，基于ESP32开发板和Arduino Framework开发，实现数字化的竹节人对战比赛自动量化记分功能，可根据分数自动裁决胜负并记录。

## 功能 | Features

- RGB LED比赛状态显示，具备多种灯效
- 传感器自动得分判定，ESP32自动裁决胜负
- 比赛定时器，用于比赛倒计时
- 自定义串口通信协议，可以输出系统状态和关键数据，并受外部控制
	- 可与任何支持串口的上位机联动（如LabVIEW）

## 硬件 | Hardware

- 竹节人套件
- 传感器件
- WS2812B LED灯带（7个灯珠）
- ESP32 dev module
- USB数据线
- USB2TTL
- 5V 2A 电源

## 软件 | Software

- VSCode + PlatformIO扩展
- LabVIEW 2018

## 依赖 | Dependency

- roboticsbrno/SmartLeds@^3.1.5

## 引脚定义 | Pin Definition

项目中主要使用的引脚定义如下：

| GPIO | 宏名          | 引脚功能描述                    |
| ---- | ------------- | ------------------------------- |
| 25   | PIN_BTN_START | 开始/停止按钮引脚               |
| 21   | PIN_BTN_PAUSE | 暂停/继续按钮引脚               |
| 17   | PIN_BTN_RESET | 重置按钮引脚                    |
| 5    | PIN_PLAYER_A  | 玩家A（红方）传感器引脚         |
| 23   | PIN_PLAYER_B  | 玩家B（蓝方）传感器引脚         |
| 19   | PIN_LED       | LED灯带引脚                     |
| 33   | PIN_DEBUG_BTN | DEBUG按钮引脚，用于串口输出测试 |

注：以上除了LED灯带引脚之外，所有引脚都启用了内置上拉电阻。
