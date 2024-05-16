/*
  * Created by MakerStore 16/05/24
  * Module for displaying BME680 sensor readings that are 
  * received via CAN bus from another Arduino Uno, on an
  * LCD display
*/

#include <CAN.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include "string.h"

/* CAN Bus packet ids */
#define TEMP_ID 1
#define PRESSURE_ID 2
#define HUMIDITY_ID 3
#define GAS_ID 4

/* CAN Bus msg length can only be 8 bytes*/
#define MSG_LEN 8

// LCD display with 20 columns & 4 rows
LiquidCrystal_I2C lcd(0x27,20,4);

// BME680 sensor readings
const char temp[MSG_LEN];
const char pressure[MSG_LEN];
const char humidity[MSG_LEN];
const char gas[MSG_LEN];

/* Reads & copies the message into the sensor reading*/
void readMsg(char* sensorVal) {
  // Clearing the sensor value 
  strcpy(sensorVal, "");
  int i = 0;
  while (CAN.available()) {
    sensorVal[i] = (char)CAN.read();;
    i++;
  }
  Serial.print("Msg Received: ");
  Serial.println(sensorVal);
}

/* Receive callback that determines which sensor value will be updated */
void onReceive(int packetSize) {

  // Packet info
  Serial.print("Received packet with id 0x");
  Serial.print(CAN.packetId(), HEX);
  Serial.print(" and length ");
  Serial.println(packetSize);

  switch (CAN.packetId()) {
    case TEMP_ID:
      readMsg(temp);
      break;
    case PRESSURE_ID:
      readMsg(pressure);
      break;
    case HUMIDITY_ID:
      readMsg(humidity);
      break;
    case GAS_ID:
      readMsg(gas);
      break;
  }
}

/* Formatting & displaying the sensor values on the lcd */
void updateLCD() {
  // Clearing the screen for new readings
  lcd.clear();

  // Displaying temp
  lcd.setCursor(0, 0);
  lcd.print("Temp:");
  lcd.print(temp);
  lcd.print("C ");

  // Displaying Pressure
  lcd.setCursor(0, 1);
  lcd.print("Pressure:");
  lcd.print(pressure);
  lcd.print(" hPa");

  // Displaying Humidity
  lcd.setCursor(0, 2);
  lcd.print("Humidity:");
  lcd.print(humidity);
  lcd.print("%");

  // Displaying Gas Resistance
  lcd.setCursor(0, 3);
  lcd.print("GasRes:");
  lcd.print(gas);
  lcd.print(" KOhms");
  
  lcd.display();
}

void setup() {
  Serial.begin(9600);
  while (!Serial);

  // Start & init the CAN bus at 500 kbps
  if (!CAN.begin(500E3)) {
    Serial.println("Starting CAN failed!");
    while (1);
  }

  // Init the lcd & turn the backlight on
  lcd.init();
  lcd.backlight(); 

  // register the receive callback
  CAN.onReceive(onReceive);
}

/* Updating the lcd screen with the new sensor readings */
void loop() {
  updateLCD();
  delay(3000);
}