#include "Config.cpp"

#include "Bluetooth\Bluetooth.cpp"

/*------------------------------(初始化内容)------------------------------*/
void beginBus() // 初始化总线
{
  //SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);
  //Wire.begin(IIC_SCL, IIC_SDA);
  Serial.begin(SERIAL_BAUD);
}
void beginDrive() // 初始化外围设备
{
  BLE_being();
  BLE_scan_being();
}
/*------------------------------(初始化)------------------------------*/
void setup()
{
  beginBus();
  beginDrive();
}
/*------------------------------(主程序)------------------------------*/
void loop()
{
  BLE_scan(0);
  Serial.println("----------------------------------------------------");
  BLE_lock_trig();
  /*for (const auto &pair : BLE_ClientMap)
  {
    //Serial.printf("设备名: %s \n", pair.second.Name.c_str());
    //Serial.printf("设备地址: %s \n", pair.first.c_str());
    //Serial.printf("设备信号强度: %d \n", pair.second.RSSI);
    Serial.printf("设备平均信号强度: %d \n", pair.second.RSSI_Average);
  }*/
  Serial.println(BLE_ClientMap.size());
  Serial.println("----------------------------------------------------");
}
