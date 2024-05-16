/*
  * Created by MakerStore 16/05/24
  * Module for transmitting BME680 readings using an Arduino UNO
  * via CAN bus to another Arduino Uno
*/

#include <Wire.h>
#include "Adafruit_BME680.h"
#include "string.h"
#include <CAN.h>

#define SEALEVELPRESSURE_HPA (1013.25)

/* CAN Bus packet ids */
#define TEMP_ID 0x1
#define PRESSURE_ID 0x2
#define HUMIDITY_ID 0x3
#define GAS_ID 0x4

Adafruit_BME680 bme;

// BME680 sensor readings
String temp;
String pressure;
String humidity;
String gas;

/* Transmits a packet with the given messsage and id */
void sendMsg(int id, int size, String msg) {
  // Packet info
  Serial.print("size = ");
  Serial.println(size);
  Serial.print("Sending ");
  Serial.print(msg);
  Serial.print(" ...");

  // Creating & transmitting the packet
  CAN.beginPacket(id);
  for(int i = 0; i < size; i++) {
    Serial.print(msg[i]);
    CAN.write(msg[i]);
  }
  CAN.endPacket();

  Serial.println(" done!");
  delay(2000);
}

/* Updates the BME680 sensor readings */
void getReadings() {

  // Checking for valid readings
  if (! bme.performReading()) {
    Serial.println("Failed to perform reading :(");
    return;
  }

  temp = String(bme.temperature);
  pressure = String(bme.pressure / 100.0);
  humidity = String(bme.humidity);
  gas = String(bme.gas_resistance / 1000.0);
}

/* Sensor reading batch transmission */
void transmitReadings() {
  sendMsg(TEMP_ID, strlen(temp.c_str()), temp);
  sendMsg(PRESSURE_ID, strlen(pressure.c_str()), pressure);
  sendMsg(HUMIDITY_ID, strlen(humidity.c_str()), humidity);
  sendMsg(GAS_ID, strlen(gas.c_str()), gas);
}

void setup() {
  // BME680 init
  Serial.begin(9600);
    if (!bme.begin()) {
    Serial.println("Could not find a valid BME680 sensor, check wiring!");
    while (1);
  }

  // Start & init the CAN bus at 500 kbps
  if (!CAN.begin(500Es3)) {
    Serial.println("Starting CAN failed!");
    while (1);
  }

  // Set up oversampling and filter initialization
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150); // 320*C for 150 ms
}

/* Periodically updating & sending sensor readings */
void loop() {
  Serial.println("Transmit New Readings!");
  getReadings();
  transmitReadings();
  Serial.println("------------------------------");
  delay(8000);
}
