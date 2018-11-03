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
//Stores data Sample 3 values
int8_t emgSample3[8];
//Stores data Sample 4 values
int8_t emgSample4[8];
//Stores data Sample 5 values
int8_t emgSample5[8];
//Stores data Sample 6 values
int8_t emgSample6[8];
//Stores data Sample 7 values
int8_t emgSample7[8];
//Stores data Sample 8 values
int8_t emgSample8[8];

size_t lengthImuData=0;

float_t accData[3]={0};

uint8_t timeEntered1=0;
uint8_t timeEntered2=0;
uint8_t timeEntered3=0;
uint8_t timeEntered4=0;
uint8_t timeEntered5=0;

int16_t convertUint8toInt16 (uint8_t leastSignificantByte, uint8_t mostSignificantByte )
{
  int16_t retVal=0;
  int16_t aux=0;

  aux= mostSignificantByte << 8; 

  retVal = aux | leastSignificantByte;

  return retVal;
}

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

void notifyCallbackEMG1(BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify) 
{
  notifyCallbackGeneric(pData,length,emgSample3,emgSample4); 
  timeEntered2 += 1;
}

void notifyCallbackEMG2(BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify) 
{
  notifyCallbackGeneric(pData,length,emgSample5,emgSample6);
  timeEntered3 += 1;
}

void notifyCallbackEMG3(BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify) 
{
  notifyCallbackGeneric(pData,length,emgSample7,emgSample8);
  timeEntered4 += 1;
}

/*
void notifyCallbackIMU(BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify) 
{
  accData[0] = (float_t) (convertUint8toInt16(*(pData + 8), *(pData + 9)))/2048;
  accData[1] = (float_t) (convertUint8toInt16(*(pData + 10), *(pData + 11)))/2048;
  accData[2] = (float_t) (convertUint8toInt16(*(pData + 12), *(pData + 13)))/2048;
  timeEntered5 += 1;
}
*/
void setup()
{
  Serial.begin(115200);
}

void loop()
{
  static bool myoConnected = 0;
  //Connect to device
  //myo.connect();
  
  if(!FirstTime) {

    //Set Notification Function Emg0Data    
    //myo.pClient->getService(BLEUUID(EMG_UUID_SERVICE))->getCharacteristic(BLEUUID(EMG0_UUID_CHARACTERISTIC))->registerForNotify(notifyCallbackEMG0);
    //Set Notification Function Emg1Data
    //myo.pClient->getService(BLEUUID(EMG_UUID_SERVICE))->getCharacteristic(BLEUUID(EMG1_UUID_CHARACTERISTIC))->registerForNotify(notifyCallbackEMG1); 
    //Set Notification Function Emg2Data
    //myo.pClient->getService(BLEUUID(EMG_UUID_SERVICE))->getCharacteristic(BLEUUID(EMG2_UUID_CHARACTERISTIC))->registerForNotify(notifyCallbackEMG2);
    //Set Notification Function Emg3Data
    //myo.pClient->getService(BLEUUID(EMG_UUID_SERVICE))->getCharacteristic(BLEUUID(EMG3_UUID_CHARACTERISTIC))->registerForNotify(notifyCallbackEMG3);
    
    //Set Notification Function IMU data
    //myo.pClient->getService(BLEUUID(IMU_UUID_SERVICE))->getCharacteristic(BLEUUID(IMU_UUID_CHARACTERISTIC))->registerForNotify(notifyCallbackIMU);
    
    FirstTime = 1;     
  }  

  /*if ((myo.connected) && (myoConnected == 0))
  {
    //Set modes on Myo: 0x01 command, 0x03 payload size, 0x02 emg mode 0x01 imu no data and 0x00 no pose indication
    uint8_t writeVal[] = {0x01, 0x03, 0x02, 0x01, 0x00};
    //Set Notification enable characteristics
    uint8_t NotifyOn = 0x01;
    //Set sleep mode
    uint8_t sleepModeVal[] = {0x09, 0x01, 0x01};
    /*
    Serial.println("Connected to Myo");
    Serial.println("");

    //Set to never sleep
    myo.pClient->getService(BLEUUID(CONTROL_UUID_SERVICE))->getCharacteristic(BLEUUID(COMMAND_UUID_CHARACTERISTIC))->writeValue(sleepModeVal, sizeof(sleepModeVal));
    delay(1000);
    Serial.println("Never Sleep configuration established");

    //Write EMG mode on Myo
    myo.pClient->getService(BLEUUID(CONTROL_UUID_SERVICE))->getCharacteristic(BLEUUID(COMMAND_UUID_CHARACTERISTIC))->writeValue(writeVal, sizeof(writeVal)); 
    delay(1000);
    Serial.println("EMG filtered only mode established");
    
    //Once mode is set, configure characterisitc to notification
    myo.pClient->getService(BLEUUID(EMG_UUID_SERVICE))->getCharacteristic(BLEUUID(EMG0_UUID_CHARACTERISTIC))->getDescriptor((uint16_t) 0x2902)->writeValue(NotifyOn, sizeof(NotifyOn));
    delay(1000);
    Serial.println("EMG0 filtered data notification configured");
    /*
    myo.pClient->getService(BLEUUID(EMG_UUID_SERVICE))->getCharacteristic(BLEUUID(EMG1_UUID_CHARACTERISTIC))->getDescriptor((uint16_t) 0x2902)->writeValue(NotifyOn, sizeof(NotifyOn));
    delay(1000);
    Serial.println("EMG1 filtered data notification configured");
    myo.pClient->getService(BLEUUID(EMG_UUID_SERVICE))->getCharacteristic(BLEUUID(EMG2_UUID_CHARACTERISTIC))->getDescriptor((uint16_t) 0x2902)->writeValue(NotifyOn, sizeof(NotifyOn));
    delay(1000);
    Serial.println("EMG2 filtered data notification configured");
    myo.pClient->getService(BLEUUID(EMG_UUID_SERVICE))->getCharacteristic(BLEUUID(EMG3_UUID_CHARACTERISTIC))->getDescriptor((uint16_t) 0x2902)->writeValue(NotifyOn, sizeof(NotifyOn));
    delay(1000);
    Serial.println("EMG3 filtered data notification configured");
    delay(1000);

    /* IMU Data accelerometer: not needed at moment
    myo.pClient->getService(BLEUUID(IMU_UUID_SERVICE))->getCharacteristic(BLEUUID(IMU_UUID_CHARACTERISTIC))->getDescriptor((uint16_t) 0x2902)->writeValue(NotifyOn, sizeof(NotifyOn));
    delay(1000);
    Serial.println("IMU data notification configured");
    delay(1000);
    */
   /*
    Serial.println("");
    Serial.println("");
    Serial.println("**********************************************");
    Serial.println("*   CONFIG FINNISHED - STARTING PROGRAM...   *");
    Serial.println("**********************************************");

    delay(3000);
    */
    //myoConnected = 1;
  //}

  timeEntered1 += 1;
  Serial.println(timeEntered1);

  /*IMU: Print accelerometer data
  Serial.print("Acc information: Times entered:  ");
  Serial.print(timeEntered5);
  Serial.print("  Sensor Data [X Y Z] position: [");
  Serial.print(accData[0]);
  Serial.print("  ");
  Serial.print(accData[1]);
  Serial.print("  ");
  Serial.print(accData[2]);
  Serial.println(" ]");
  */

  delay(50);
}
 