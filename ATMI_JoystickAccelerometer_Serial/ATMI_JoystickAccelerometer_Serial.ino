/*
  Teaching Creative Coding and Expressive Instrument Design with SparkFun Qwiic Connect Sensors and Max/MSP
  ATMI Workshop Code 
  Steven Kemper
  11/7/2024

  Based on:
  Serial Call and Response
  Language: Wiring/Arduino
  created 26 Sep 2005
  by Tom Igoe
  modified 24 Apr 2012
  by Tom Igoe and Scott Fitzgerald
  Thanks to Greg Shakar and Scott Fitzgerald for the improvements

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/SerialCallResponse


*/

#include <Wire.h>
#include "SparkFun_KX13X.h" //Accelerometer Library (download from Arduino Library Manager
#include "SparkFun_Qwiic_Joystick_Arduino_Library.h" //Joystick Library (download from Arduino Library Manager

SparkFun_KX132 kxAccel; //Create instance of accelerometer  object

JOYSTICK joystick; //Create instance of joystick object


outputData myData;  // This will store the accelerometer's output.

//Variables to store joystick data
int joystick_X = 0;
int joystick_Y = 0;
int joystick_Button = 0;


int inByte = 0;  // incoming serial byte

void setup() {
  Wire.begin();
  Serial.begin(19200);
  Serial.println("Welcome.");

  // Wait for the Serial monitor to be opened.
  while (!Serial)
    delay(50);

if(joystick.begin() == false)
  {
    Serial.println("Joystick does not appear to be connected. Please check wiring. Freezing...");
    while(1);
  }

  if (!kxAccel.begin()) {
    Serial.println("Could not communicate with the the KX13X. Freezing.");
    while (1)
      ;
  }

  Serial.println("Ready.");

  if (kxAccel.softwareReset())
    Serial.println("Reset.");

  // Many settings for KX13X can only be
  // applied when the accelerometer is powered down.
  // However there are many that can be changed "on-the-fly"
  // check datasheet for more info, or the comments in the
  // "...regs.h" file which specify which can be changed when.
  kxAccel.enableAccel(false);

  kxAccel.setRange(SFE_KX132_RANGE2G);      // 2g Range
  kxAccel.enableDataEngine();  // Enables the bit that indicates data is ready.
  // kxAccel.setOutputDataRate(); // Default is 50Hz
  kxAccel.enableAccel();


  establishContact();  // send a byte to establish contact until receiver responds
}

void loop() {
  // if we get a valid byte, read analog ins:
  if (Serial.available() > 0) {
    // get incoming byte:
    inByte = Serial.read();
    joystick_X = joystick.getHorizontal();
    joystick_Y = joystick.getVertical();
    joystick_Button = joystick.getButton(); 
    
    // send sensor values to Max:
    kxAccel.getAccelData(&myData); 
    Serial.print((myData.xData * 100), 4);  //Accelerometer X data, scaling from float to int values
    Serial.print(", ");             //use comma as delimiter (to separate values)
    Serial.print((myData.yData * 100), 4);  //Accel Y
    Serial.print(", ");             //use comma as delimiter (to separate values)
    Serial.print((myData.zData * 100), 4);  //Accel Z
    Serial.print(", ");             //use comma as delimiter (to separate values)
    Serial.print(joystick_X);       //send Joystick X position to serial buffer
    Serial.print(", ");             //use comma as delimiter (to separate values)
    Serial.print(joystick_Y);       //send Joystick Y position to serial buffer
    Serial.print(", ");             //use comma as delimiter (to separate values)
    Serial.println(joystick_Button);     //send Joystick button state to serial buffer, print line end
    

    delay(20);  // 
  }
}

void establishContact() {
  while (Serial.available() <= 0) {
    Serial.print('A');  // send a capital A
    delay(300);
  }
}
