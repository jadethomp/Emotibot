//This example code is in the Public Domain (or CC0 licensed, at your option.)
//By Evandro Copercini - 2018
//
//This example creates a bridge between Serial and Classical Bluetooth (SPP)
//and also demonstrate that SerialBT have the same functionalities of a normal Serial

#include "BluetoothSerial.h"
#include <Wire.h>
#include "Adafruit_DRV2605.h"
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif
#include <Adafruit_BNO055.h>

//#define USE_PIN // Uncomment this to use PIN during pairing. The pin is specified on the line below
const char *pin = "1234"; // Change this to more secure PIN.

#define USE_DRV // Uncomment this to use DRV for heartbeat feedback
#define DEPLOY // Uncomment this to deploy full feedback array

//Emotion Encoding Scheme ------------
//0: Anxious
//1: Sad
//2: Happy
int current_emotion = 2;
//BPM array
int emotions[3] = {110, 50, 75};

String device_name = "BonBon2";

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Bluetooth not available or not enabled. It is only available for the ESP32 chip.
#endif

BluetoothSerial SerialBT;

// Heartbeat -------------

Adafruit_DRV2605 drv;

// DRV2605 vibration settings (see chart of all settings on adafruit datasheet)
// use 17 and 19 for ERM, 8 and 9 for LRA
uint8_t loud = 17;
uint8_t soft = 19;

// delay between a beat's two vibrations
const uint8_t shortDecDelay = 142;
// delay between beats
uint8_t decDelay = 163; // starting BPM of 100

int BPMtoSpacing(int bpm){
  int beatLength, decDelay, delay;
  beatLength = 60000 / bpm;
  delay = beatLength - 250;
  decDelay = (delay / 10) + 128;
  return decDelay; // not in ms - waveform setting value
}

int spacingToBPM(int decDelay){
  int delay, beatLength, bpm;
  delay = (decDelay - 128) * 10; // change to true delay in ms
  beatLength = delay + 250;
  bpm = 60000 / beatLength;
  return bpm;
}

// sets the heartbeat sequence (vibrations and delays strung together)
void setSequence(int delay){
  drv.setWaveform(0, loud);
  drv.setWaveform(1, shortDecDelay);
  drv.setWaveform(2, soft);
  drv.setWaveform(3, delay);
  drv.setWaveform(4, 0);
}

// NeoPixel -----------
// apparently CANNOT be 12 or will get stuck in boot loop :P
#define LED_PIN    14

// How many NeoPixels are attached to the Arduino?
#define LED_COUNT 4

int FADE_DELAY = 10;
unsigned long CURRENT_TIME = millis();

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_RGBW + NEO_KHZ800);
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)

// brighten() and darken() functions adapted from https://forum.arduino.cc/t/adafruit-neopixel-code-for-simple-brightness-fade/418170/2 
// 0 to 255
void brighten() {
  uint16_t i, j;

  for (j = 10; j < 255; j++) {
    for (i = 1; i < strip.numPixels(); i+=2) {
      drv.go();
      if(current_emotion == 0){ //anxious, red
        strip.setPixelColor(i, 0, j, 0, 1);
      }
      else if(current_emotion == 1){ //sad, blue
        strip.setPixelColor(i, 0, 0, j, 1);
      }
      else if(current_emotion == 2){ //happy, green
        strip.setPixelColor(i, j, 0, 0, 1);
      }
    }
    strip.show();
    delay(FADE_DELAY);
  }
}

// 255 to 0
void darken() {
  uint16_t i, j;

  for (j = 255; j > 10; j--) {
    for (i = 1; i < strip.numPixels(); i+=2) {
      drv.go();
      if(current_emotion == 0){ //anxious, red
        strip.setPixelColor(i, 0, j, 0, 1);
      }
      else if(current_emotion == 1){ //sad, blue
        strip.setPixelColor(i, 0, 0, j, 1);
      }
      else if(current_emotion == 2){ //happy, green
        strip.setPixelColor(i, j, 0, 0, 1);
      }
    }
    strip.show();
    delay(FADE_DELAY);
  }
}

void emoCheck(int input)
{
  //when person is present
  if(input == 'h')
  {
    Serial.println("Bonbon is really scared of people ...");
    SerialBT.write('h');
    current_emotion = 0;
  }
  //When no one is present
  else if(input == 'n')
  {
    Serial.println("Bonbon is happy they're alone!!");
    SerialBT.write('n');
    current_emotion = 2;
  }
  else
  {
    Serial.println("Parse Failed. Value: " + input);
  }
  FADE_DELAY = (current_emotion == 0) ? 1 : (current_emotion == 1) ? 7 : 5;
}

// dual core init
TaskHandle_t Task1;

// IMU -------
double xPos = 0, yPos = 0, current_xpos = 0, current_ypos = 0, last_xpos = 0, last_ypos = 0;
uint16_t BNO055_SAMPLERATE_DELAY_MS = 10; //how often to read data from the board
uint16_t PRINT_DELAY_MS = 500; // how often to print the data
uint16_t printCount = 0; //counter to avoid printing every 10MS sample

double ACCEL_VEL_TRANSITION =  (double)(BNO055_SAMPLERATE_DELAY_MS) / 1000.0;
double ACCEL_POS_TRANSITION = 0.5 * ACCEL_VEL_TRANSITION * ACCEL_VEL_TRANSITION;
double DEG_2_RAD = 0.01745329251; //trig functions require radians, BNO055 outputs degrees

double MAX_DIFF = 50.0;
int imu_checks = 0, stillness = 0;

Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);

void setup() {
  Serial.begin(115200);
  SerialBT.begin(device_name); //Bluetooth device name

  Serial.printf("The device with name \"%s\" and BT address %s is started.\nNow you can pair it with Bluetooth!\n", device_name.c_str(), SerialBT.getBtAddressString()); // Use this after the MAC method is implemented
  #ifdef USE_PIN
    SerialBT.setPin(pin);
    Serial.println("Using PIN");
  #endif

  #ifdef USE_DRV
    if (! drv.begin()) {
      Serial.println("Could not find DRV2605");
      while (1) delay(10);
    }
  
    drv.selectLibrary(1);
    
    // I2C trigger by sending 'go' command 
    // default, internal trigger when sending GO command
    drv.setMode(DRV2605_MODE_INTTRIG);
  #endif

  #ifdef DEPLOY
    strip.begin();
    strip.show();
    strip.setBrightness(255);
  #endif

  if(!bno.begin()){
    Serial.println("No IMU detected");
    while(1);
  }
  Serial.println("IMU began!");

  // dual core setup
  // code is pinned to core 1 by default, so core 0 is the additional one here
  // (dual core info here: https://randomnerdtutorials.com/esp32-dual-core-arduino-ide/)
  xTaskCreatePinnedToCore(bluetooth_handler, "bluetooth", 10000, NULL, 1, &Task1, 0);
}

void bluetooth_handler(void * pvParameters){
  Serial.print("Bluetooth handler running on core ");
  Serial.println(xPortGetCoreID());

  // recreate a void loop() for core 0
  for(;;){
    //Check if arduino is trying to send code to pi
    if (Serial.available()) {
      SerialBT.write(Serial.read());
    }
    if (SerialBT.available()) {
      emoCheck(SerialBT.read());
    }
    delay(20);

    // IMU signal processing
    unsigned long start_time = micros();
    sensors_event_t orientation_data, linear_accel_data;
    bno.getEvent(&orientation_data, Adafruit_BNO055::VECTOR_EULER);
    // bno.getEvent(&linear_accel_data, Adafruit_BNO055::VECTOR_LINEARACCEL);

    // velocity of sensor in the direction it's facing
    // speed = ACCEL_VEL_TRANSITION * linear_accel_data.acceleration.x / cos(DEG_2_RAD * orientation_data.orientation.x);

    // xPos = xPos + ACCEL_POS_TRANSITION * linear_accel_data.acceleration.x;
    // yPos = yPos + ACCEL_POS_TRANSITION * linear_accel_data.acceleration.y;

    if (printCount * BNO055_SAMPLERATE_DELAY_MS >= PRINT_DELAY_MS) {
      current_xpos = orientation_data.orientation.x;
      current_ypos = orientation_data.orientation.y;

      // Serial.println("Checking...");
      // Serial.print(last_xpos);
      // Serial.print(" - ");
      // Serial.println(current_xpos);
      // Serial.print(last_ypos);
      // Serial.print(" - ");
      // Serial.println(current_ypos);
      
      if((abs(last_xpos - current_xpos) > MAX_DIFF || abs(last_ypos - current_ypos) > MAX_DIFF) && imu_checks > 5){ // arbitrary value so first few are ignored
        // set emo to presence
        emoCheck('h');
        stillness = 0;
      }
      else if(abs(last_xpos - current_xpos) < 1.0 && abs(last_ypos - current_ypos) < 1.0){
        stillness++;
      }

      if(stillness > 5){ // again, arbitrary value
        emoCheck('n');
        stillness = 0;
      }
      // if IMU crash thingy happens
      if(current_xpos == 0.0 || current_ypos == 0.0){
        Serial.println("resetting IMU");
        bno.begin(); // maybe this resets who knows
        imu_checks = 0;

      }

      imu_checks++;
      printCount = 0;
      last_xpos = current_xpos;
      last_ypos = current_ypos;
    }
    else {
      printCount = printCount + 1;
    }
  }
}

void loop() {
  #ifdef DEPLOY
    strip.show();

    // if(millis() >= CURRENT_TIME + 5000){
    //   current_emotion = random(3);
    //   FADE_DELAY = (current_emotion == 0) ? 1 : (current_emotion == 1) ? 7 : 5;
    //   CURRENT_TIME = millis();
    // }
    
    decDelay = BPMtoSpacing(emotions[current_emotion]);
    setSequence(decDelay);
    drv.go();
    // these would be better if they were like callbacks but who has time for that
    brighten();
    darken();
  #endif

  // BT serial moved to core 0
}
