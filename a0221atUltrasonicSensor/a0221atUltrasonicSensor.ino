"""
Created by MakerStore on the 30/04/2024

Module for calculating the distance (cm) using an A0221AT ultrasonic sensor
on an Arduino Uno

"""

#include <SoftwareSerial.h>
#include <Wire.h>

// Connect the sensor's RX/TX pin to these Arduino pins
#define RXpin 2
#define TXpin 3
SoftwareSerial mySerial(RXpin, TXpin);
unsigned char data[4] = {};
float distance = 0.00;

void setup() {
  pinMode(RXpin, INPUT);
  pinMode(TXpin, OUTPUT);
  Wire.begin();
  Serial.begin(9600);
  mySerial.begin(9600);
}

void loop() {
  delay(30);

  // Resetting the RX pin
  digitalWrite(TXpin, HIGH);
  delay(30);

  delay(2); // Ensure the pulse is detected
  digitalWrite(TXpin, LOW);
  delay(30);

  // Send positive pulse to the sensor's RX pin to start measurement
  digitalWrite(TXpin, HIGH);

  // Wait for the measurement to complete
  delay(60); // Adjust delay based on sensor specifications

  // Gathering & calculating the data
  for(int i=0; i<4; i++) { data[i] = mySerial.read(); }
  distance = ((data[1]*256)+data[2] )/ 10;

  // Display the measurement
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Adjust delay based on measurement frequency
  delay(5000);
}
