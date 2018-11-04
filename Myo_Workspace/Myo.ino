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

//Each sample is acquired 5 ms 
//Stores data Sample 1 values
int8_t emgSample1[8];
//Stores data Sample 2 values
int8_t emgSample2[8];


uint8_t timeEntered1=0;
uint8_t timeEntered2=0;
uint8_t timeEntered3=0;
uint8_t timeEntered4=0;
uint8_t timeEntered5=0;

void notifyCallbackGeneric(uint8_t* pData, size_t length, int8_t * firstSample, int8_t *secondSample)
{
  //Pointer to store data in first or second sample array
  int8_t *arrayPointer;

  int indexData;
  arrayPointer=  firstSample;

  for(indexData=0; indexData<length; indexData++) 
  {
    arrayPointer[indexData%8] = (int8_t) pData[indexData];
    if (indexData==8)
    {
      arrayPointer =  secondSample;
    }
  }
}

void notifyCallbackEMG0(BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify) 
{ 
  notifyCallbackGeneric(pData,length,emgSample1,emgSample2); 
  timeEntered1 += 1;
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
    myo.pClient->getService(BLEUUID(EMG_UUID_SERVICE))->getCharacteristic(BLEUUID(EMG0_UUID_CHARACTERISTIC))->registerForNotify(notifyCallbackEMG0);
    
    FirstTime = 1;     
  }  

  if ((myo.connected) && (myoConnected == 0))
  {
    //Set modes on Myo: 0x01 command, 0x03 payload size, 0x02 emg mode 0x01 imu no data and 0x00 no pose indication
    uint8_t writeVal[] = {0x01, 0x03, 0x02, 0x01, 0x00};
    //Set Notification enable characteristics
    uint8_t NotifyOn = 0x01;
    //Set sleep mode
    uint8_t sleepModeVal[] = {0x09, 0x01, 0x01};
    //Set Vibration
    uint8_t vibrationMode[] = {0x03, 0x01, 0x02};
    uint8_t noVibrationMode[] = {0x03, 0x01, 0x00};
    //Set to never sleep
    myo.pClient->getService(BLEUUID(CONTROL_UUID_SERVICE))->getCharacteristic(BLEUUID(COMMAND_UUID_CHARACTERISTIC))->writeValue(sleepModeVal, sizeof(sleepModeVal));
    delay(1000);

    //Write EMG mode on Myo
    myo.pClient->getService(BLEUUID(CONTROL_UUID_SERVICE))->getCharacteristic(BLEUUID(COMMAND_UUID_CHARACTERISTIC))->writeValue(writeVal, sizeof(writeVal)); 
    delay(1000);
    
    //Once mode is set, configure characterisitc to notification
    myo.pClient->getService(BLEUUID(EMG_UUID_SERVICE))->getCharacteristic(BLEUUID(EMG0_UUID_CHARACTERISTIC))->getDescriptor((uint16_t) 0x2902)->writeValue(NotifyOn, sizeof(NotifyOn));
    delay(1000);

    myoConnected = 1;

    //Vibrate to indicate configuration is finnished
    myo.pClient->getService(BLEUUID(CONTROL_UUID_SERVICE))->getCharacteristic(BLEUUID(COMMAND_UUID_CHARACTERISTIC))->writeValue(vibrationMode,sizeof(vibrationMode));
    delay(1000);
    //En vibration mode
    myo.pClient->getService(BLEUUID(CONTROL_UUID_SERVICE))->getCharacteristic(BLEUUID(COMMAND_UUID_CHARACTERISTIC))->writeValue(noVibrationMode,sizeof(noVibrationMode));
    delay(1000);
  }
  
  Serial.print(emgSample1[0]);
  Serial.print("\t");
  Serial.print(emgSample1[1]);
  Serial.print("\t");
  Serial.print(emgSample1[2]);
  Serial.print("\t");
  Serial.print(emgSample1[3]);
  Serial.print("\t");
  Serial.print(emgSample1[4]);
  Serial.print("\t");
  Serial.print(emgSample1[5]);
  Serial.print("\t");
  Serial.print(emgSample1[6]);
  Serial.print("\t");
  Serial.println(emgSample1[7]);

  delay(50);
}
 