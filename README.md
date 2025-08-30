# arduino_esp32_zjr - Arduino ESP32 竹节人大作战数字擂台

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
- RGB LED灯带
- ESP32 dev module
- USB数据线
- USB2TTL
- 5V 2A 电源

## 软件 | Software

- VSCode + PlatformIO插件
- LabVIEW 2018
