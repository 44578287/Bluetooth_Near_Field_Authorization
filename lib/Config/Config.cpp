#pragma once
// 外部引用
#include <Arduino.h>
#include <string>
#include <map>
#include <vector>
//硬件库
#include <BLEDevice.h> // 蓝牙Ble设备库
#include <BLEUtils.h>
#include <BLEScan.h>             // 蓝牙ble设备的扫描功能库 本篇重点
#include <BLEAdvertisedDevice.h> // 扫描到的蓝牙设备（广播状态）

using namespace std;
/*------------------------------(串口)------------------------------*/
#define SERIAL_BAUD 115200 // 串口波特率

/*------------------------------(蓝牙)------------------------------*/
#define BLE_NAME "ESP32 近场授权" // 蓝牙设备名称
#define BLE_SCANTIME 2            // In seconds
BLEScan *pBLEScan;                // 扫描对象
typedef struct
{
    String Name; // 设备名
    String MAC;  // 设备地址
    int RSSI;           // 设备信号强度
    int RSSI_Average;   // 平均信号强度
} BLE_Client;           // 蓝牙客户端
std::map<String, BLE_Client> BLE_ClientMap; // 蓝牙客户端列表<MAC地址,设备信息>
std::vector<String> BLE_Client_BanList; // 蓝牙客户端黑名单<MAC地址>
#define BLE_LOCK_VALUE -55 // 蓝牙锁定值
#define BLE_LOCK_VALUE_AVERAGE -55 // 蓝牙锁定平均值