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
int8_t dataSample1[8];
//Stores data Sample 2 values
int8_t dataSample2[8];
//Stores data Sample 3 values
int8_t dataSample3[8];
//Stores data Sample 4 values
int8_t dataSample4[8];
//Stores data Sample 5 values
int8_t dataSample5[8];
//Stores data Sample 6 values
int8_t dataSample6[8];
//Stores data Sample 7 values
int8_t dataSample7[8];
//Stores data Sample 8 values
int8_t dataSample8[8];

uint8_t timeEntered1=0;
uint8_t timeEntered2=0;
uint8_t timeEntered3=0;
uint8_t timeEntered4=0;

void notifyCallbackGeneric(uint8_t* pData, size_t length, int8_t * firstSample, int8_t *secondSample)
{
  //Pointer to store data in first or second sample array
  int8_t *arrayPointer;

  int indexData;
  arrayPointer=  firstSample;
  //Serial.print("Sensors reading:  ");
  //Serial.print("First Sample: ");
  for(indexData=0; indexData<length; indexData++) 
  {
    arrayPointer[indexData%8] = (int8_t) pData[indexData];
    if (indexData==8)
    {
      arrayPointer =  secondSample;
      //Serial.println("");
      //Serial.print("Sensors reading:  ");
      //Serial.print("Second Sample: ");
    }
    //Serial.print(indexData);
    //Serial.print(": ");
    //Serial.print(arrayPointer[indexData%8]);
    //Serial.print(" ");
  }
  //Serial.println("");
  //Serial.print("  Finnished sensor reading with number : ");
  //Serial.println(indexData);
  //Serial.println("");
  //Serial.println("");
}

void notifyCallbackEMG0(BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify) 
{ 
  notifyCallbackGeneric(pData,length,dataSample1,dataSample2); 
  timeEntered1 += 1;
}

void notifyCallbackEMG1(BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify) 
{
  notifyCallbackGeneric(pData,length,dataSample3,dataSample4); 
  timeEntered2 += 1;
}

void notifyCallbackEMG2(BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify) 
{
  notifyCallbackGeneric(pData,length,dataSample5,dataSample6);
  timeEntered3 += 1;
}

void notifyCallbackEMG3(BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify) 
{
  notifyCallbackGeneric(pData,length,dataSample7,dataSample8);
  timeEntered4 += 1;
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
    myo.pClient->getService(BLEUUID("d5060005-a904-deb9-4748-2c7f4a124842"))->getCharacteristic(BLEUUID("d5060105-a904-deb9-4748-2c7f4a124842"))->registerForNotify(notifyCallbackEMG0);
    //Set Notification Function Emg1Data
    myo.pClient->getService(BLEUUID("d5060005-a904-deb9-4748-2c7f4a124842"))->getCharacteristic(BLEUUID("d5060205-a904-deb9-4748-2c7f4a124842"))->registerForNotify(notifyCallbackEMG1); 
    //Set Notification Function Emg2Data
    myo.pClient->getService(BLEUUID("d5060005-a904-deb9-4748-2c7f4a124842"))->getCharacteristic(BLEUUID("d5060305-a904-deb9-4748-2c7f4a124842"))->registerForNotify(notifyCallbackEMG2);
    //Set Notification Function Emg3Data
    myo.pClient->getService(BLEUUID("d5060005-a904-deb9-4748-2c7f4a124842"))->getCharacteristic(BLEUUID("d5060405-a904-deb9-4748-2c7f4a124842"))->registerForNotify(notifyCallbackEMG3);
    FirstTime = 1;     
  }  

  if ((myo.connected) && (myoConnected == 0))
  {
    //Set modes on Myo: 0x01 command, 0x03 payload size, 0x02 emg mode 0x00 imu no data and 0x00 no pose indication
    uint8_t writeVal[] = {0x01, 0x03, 0x02, 0x00, 0x00};
    //Set Notification enable characteristics
    uint8_t NotifyOn = 0x01;
    //Set sleep mode
    uint8_t sleepModeVal[] = {0x09, 0x01, 0x01};

    Serial.println("Connected to Myo");
    Serial.println("");

    //Set to never sleep
    myo.pClient->getService(BLEUUID("d5060001-a904-deb9-4748-2c7f4a124842"))->getCharacteristic(BLEUUID("d5060401-a904-deb9-4748-2c7f4a124842"))->writeValue(sleepModeVal, sizeof(sleepModeVal));
    delay(1000);
    Serial.println("Never Sleep configuration established");

    //Write EMG mode on Myo
    myo.pClient->getService(BLEUUID("d5060001-a904-deb9-4748-2c7f4a124842"))->getCharacteristic(BLEUUID("d5060401-a904-deb9-4748-2c7f4a124842"))->writeValue(writeVal, sizeof(writeVal)); 
    delay(1000);
    Serial.println("EMG filtered only mode established");
    
    //Once mode is set, configure characterisitc to notification
    myo.pClient->getService(BLEUUID("d5060005-a904-deb9-4748-2c7f4a124842"))->getCharacteristic(BLEUUID("d5060105-a904-deb9-4748-2c7f4a124842"))->getDescriptor((uint16_t) 0x2902)->writeValue(NotifyOn, sizeof(NotifyOn));
    delay(1000);
    Serial.println("EMG0 filtered data notification configured");
    myo.pClient->getService(BLEUUID("d5060005-a904-deb9-4748-2c7f4a124842"))->getCharacteristic(BLEUUID("d5060205-a904-deb9-4748-2c7f4a124842"))->getDescriptor((uint16_t) 0x2902)->writeValue(NotifyOn, sizeof(NotifyOn));
    delay(1000);
    Serial.println("EMG1 filtered data notification configured");
    myo.pClient->getService(BLEUUID("d5060005-a904-deb9-4748-2c7f4a124842"))->getCharacteristic(BLEUUID("d5060305-a904-deb9-4748-2c7f4a124842"))->getDescriptor((uint16_t) 0x2902)->writeValue(NotifyOn, sizeof(NotifyOn));
    delay(1000);
    Serial.println("EMG2 filtered data notification configured");
    myo.pClient->getService(BLEUUID("d5060005-a904-deb9-4748-2c7f4a124842"))->getCharacteristic(BLEUUID("d5060405-a904-deb9-4748-2c7f4a124842"))->getDescriptor((uint16_t) 0x2902)->writeValue(NotifyOn, sizeof(NotifyOn));
    delay(1000);
    Serial.println("EMG3 filtered data notification configured");
    delay(1000);

    Serial.println("");
    Serial.println("");
    Serial.println("**********************************************");
    Serial.println("*   CONFIG FINNISHED - STARTING PROGRAM...   *");
    Serial.println("**********************************************");

    delay(3000);

    myoConnected = 1;
  }

  Serial.println("");
  Serial.println("");

  Serial.print("EMG0 information: Times entered:  ");
  Serial.print(timeEntered1);
  Serial.print("  Sensor Data 1 position:  ");
  Serial.println(dataSample1[0]);

  Serial.print("EMG1 information: Times entered:  ");
  Serial.print(timeEntered2);
  Serial.print("  Sensor Data 1 position:  ");
  Serial.println(dataSample3[0]);

  Serial.print("EMG2 information: Times entered:  ");
  Serial.print(timeEntered3);
  Serial.print("  Sensor Data 1 position:  ");
  Serial.println(dataSample5[0]);

  Serial.print("EMG3 information: Times entered:  ");
  Serial.print(timeEntered4);
  Serial.print("  Sensor Data 1 position:  ");
  Serial.println(dataSample7[0]);

  Serial.println("");
  Serial.println("");

  delay(1000);
  //Vibrate myo
  //myo.pClient->getService(BLEUUID("d5060001-a904-deb9-4748-2c7f4a124842"))->getCharacteristic(BLEUUID("d5060401-a904-deb9-4748-2c7f4a124842"))->writeValue(0x03);
}
