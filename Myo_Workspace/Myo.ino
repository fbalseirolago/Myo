#include "myo.h"
#include <BLEDevice.h>
#include <BLEClient.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <stdint.h>
#include <BLEValue.h>

myo myo;

bool FirstTime = 0;

void notifyCallbackEMG01(BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify) {
  int indexData;

  //Stores data Sample 1 values
  int8_t dataSample1[8];
  //Stores data Sample 2 values
  int8_t dataSample2[8];

  //Pointer to store data in array 1 or 2
  int8_t *arrayPointer;
  
  arrayPointer=  dataSample1;
  Serial.print("Sensors reading:  ");
  Serial.print("First Sample: ");
  for(indexData=0; indexData<length; indexData++) 
  {
    arrayPointer[indexData%8] = (int8_t) pData[indexData];
    if (indexData==8)
    {
      arrayPointer =  dataSample2;
      Serial.println("");
      Serial.print("Sensors reading:  ");
      Serial.print("Second Sample: ");
    }
    Serial.print(indexData);
    Serial.print(": ");
    Serial.print(arrayPointer[indexData%8]);
    Serial.print(" ");
  }
  Serial.println("");
  Serial.print("  Finnished sensor reading with number : ");
  Serial.println(indexData);
  Serial.println("");
  Serial.println("");
}

void setup()
{
  Serial.begin(115200);
}

void loop()
{
  static bool myoConnected = 0;
  //Connect to device
  myo.connect();
  
  if(!FirstTime) {

    //Set Notification Function Emg0Data    
    myo.pClient->getService(BLEUUID("d5060005-a904-deb9-4748-2c7f4a124842"))->getCharacteristic(BLEUUID("d5060105-a904-deb9-4748-2c7f4a124842"))->registerForNotify(notifyCallbackEMG01); 
    FirstTime = 1;     
  }  

  if ((myo.connected) && (myoConnected == 0))
  {
    //Set modes on Myo: 0x01 command, 0x03 payload size, 0x02 emg mode 0x00 imu no data and 0x00 no pose indication
    uint8_t writeVal[] = {0x01, 0x03, 0x02, 0x00, 0x00};
    uint8_t NotifyOn = 0x01;
    
    //Write mode on Myo
    myo.pClient->getService(BLEUUID("d5060001-a904-deb9-4748-2c7f4a124842"))->getCharacteristic(BLEUUID("d5060401-a904-deb9-4748-2c7f4a124842"))->writeValue(writeVal, sizeof(writeVal)); 
    
    //Once connected, configure characterisitc to notification
    myo.pClient->getService(BLEUUID("d5060005-a904-deb9-4748-2c7f4a124842"))->getCharacteristic(BLEUUID("d5060105-a904-deb9-4748-2c7f4a124842"))->getDescriptor((uint16_t) 0x2902)->writeValue(NotifyOn, sizeof(NotifyOn));
    Serial.print("Connected to Myo");
    myoConnected = 1;
  }

  delay(1000);
  //Vibrate myo
  //myo.pClient->getService(BLEUUID("d5060001-a904-deb9-4748-2c7f4a124842"))->getCharacteristic(BLEUUID("d5060401-a904-deb9-4748-2c7f4a124842"))->writeValue(0x03);
}
