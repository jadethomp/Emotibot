//This example code is in the Public Domain (or CC0 licensed, at your option.)
//By Evandro Copercini - 2018
//
//This example creates a bridge between Serial and Classical Bluetooth (SPP)
//and also demonstrate that SerialBT have the same functionalities of a normal Serial

#include "BluetoothSerial.h"

//#define USE_PIN // Uncomment this to use PIN during pairing. The pin is specified on the line below
const char *pin = "1234"; // Change this to more secure PIN.

//Emotion Encoding Scheme
//0: Neutral
//1: Happy
//2: Sad
//3: Anxious
int current_emotion = 0;

String device_name = "BonBon1";

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Bluetooth not available or not enabled. It is only available for the ESP32 chip.
#endif

BluetoothSerial SerialBT;

void setup() {
  Serial.begin(115200);
  SerialBT.begin(device_name); //Bluetooth device name

  Serial.printf("The device with name \"%s\" and BT address %s is started.\nNow you can pair it with Bluetooth!\n", device_name.c_str(), SerialBT.getBtAddressString()); // Use this after the MAC method is implemented
  #ifdef USE_PIN
    SerialBT.setPin(pin);
    Serial.println("Using PIN");
  #endif
}

void emoCheck(int input)
{
  if(input == 'h')
  {
    Serial.println("BonBon is now Happy!!");
    current_emotion = 1;
  }
  else if(input == 's')
  {
    Serial.println("BonBon is so sad :(");
    current_emotion = 2;
  }
  else if(input == 'a')
  {
    Serial.println("BonBon is really scared ...");
    current_emotion = 3;
  }
  else
  {
    Serial.println("Parse Failed. Value: " + input);
  }
}

void loop() {
  if (Serial.available()) {
    SerialBT.write(Serial.read());
  }
  if (SerialBT.available()) {
    emoCheck(SerialBT.read());
  }
  delay(20);
}
