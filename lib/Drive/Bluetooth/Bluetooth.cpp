#include "Config.cpp"

// 扫描广播设备结果回调
class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks
{
    void onResult(BLEAdvertisedDevice advertisedDevice)
    {
        //Serial.printf("宣传设备: %s \n", advertisedDevice.toString().c_str());
        //打设备名 + 设备地址 + 设备信号强度
        if (advertisedDevice.getServiceData() != "")
        {
            // Serial.printf("设备名: %s \n", advertisedDevice.getServiceData().c_str());
            // Serial.printf("设备地址: %s \n", advertisedDevice.getAddress().toString().c_str());
            // Serial.printf("设备信号强度: %d \n", advertisedDevice.getRSSI());
            // 单行美化打印
            // Serial.printf("%s --%s --%d \r\n", advertisedDevice.getServiceData().c_str(), advertisedDevice.getAddress().toString().c_str(), advertisedDevice.getRSSI());
            String Name = advertisedDevice.getServiceData().c_str();
            String MAC = advertisedDevice.getAddress().toString().c_str();
            int RSSI = advertisedDevice.getRSSI();
            Serial.printf("%s --%s --%d \r\n", Name.c_str(), MAC.c_str(), RSSI);

            //BLE_ClientMap.insert(MAC, new BLE_Client(){Name, RSSI, RSSI_Average});
            if (BLE_ClientMap.find(MAC) != BLE_ClientMap.end())
            {
                int RSSI_Average = (BLE_ClientMap[MAC].RSSI_Average + RSSI) / 2;
                BLE_Client bleClient = {Name, MAC, RSSI, RSSI_Average};
                BLE_ClientMap[MAC] = bleClient;
                Serial.printf("已有设备,更新信号强度: %s", Name);
            }
            else
            {
                BLE_Client bleClient = {Name, MAC, RSSI, RSSI};
                BLE_ClientMap.insert(std::make_pair(MAC, bleClient));
                Serial.printf("新设备: %s", Name);
            }
        }
    }
};

void BLE_being()
{
    BLEDevice::init(BLE_NAME); // 设备初始化
    BLEDevice::startAdvertising(); // 开启Advertising广播
}

void BLE_scan_being()
{
    pBLEScan = BLEDevice::getScan(); // 扫描对象
    pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks()); // 扫描结果回调
    pBLEScan->setActiveScan(true); // 扫描模式，主动扫描
    pBLEScan->setInterval(100); // 扫描间隔
    pBLEScan->setWindow(99); // 扫描窗口
}

void BLE_scan(int DelayTime)
{
    BLEScanResults foundDevices = pBLEScan->start(BLE_SCANTIME, false); // 扫描时间，重复扫描
    Serial.print("找到的设备: ");
    Serial.println(foundDevices.getCount());
    Serial.println("扫描完成!");
    pBLEScan->clearResults(); // 扫描结束，清除结果
    vTaskDelay(DelayTime / portTICK_PERIOD_MS);
}

void BLE_scan_stop()
{
    pBLEScan->stop();
}

//map<BLE_Client, int> BLE_RSSI;
std::vector<BLE_Client> BLE_RSSI;
BLE_Client BLE_lock_trig()
{
    for (const auto &pair : BLE_ClientMap)//获取符合信号强度的设备
    {
        if (pair.second.RSSI > BLE_LOCK_VALUE || pair.second.RSSI_Average > BLE_LOCK_VALUE_AVERAGE)
        {
            /*Serial.printf("设备名: %s \n", pair.second.Name.c_str());
            Serial.printf("设备地址: %s \n", pair.first.c_str());
            Serial.printf("设备信号强度: %d \n", pair.second.RSSI);
            Serial.printf("设备平均信号强度: %d \n", pair.second.RSSI_Average);*/
            BLE_RSSI.push_back(pair.second);
        }
    }
    BLE_Client minDevice = {"Null", "Null", -100, -100};
    for (auto element : BLE_RSSI)//多设备时，获取当前最强的设备
    {
        if (element.RSSI > minDevice.RSSI)
        {
            minDevice = element;
        }
    }
    if (minDevice.Name != "Null")
    {
        Serial.printf("设备名: %s \n", minDevice.Name.c_str());
        Serial.printf("设备地址: %s \n", minDevice.MAC.c_str());
        Serial.printf("设备信号强度: %d \n", minDevice.RSSI);
        Serial.printf("设备平均信号强度: %d \n", minDevice.RSSI_Average);
    }
    BLE_RSSI.clear();
    return minDevice;
}