/*  
 *  LoRa 868 / 915MHz SX1272 LoRa module
 *  
 *  Copyright (C) Libelium Comunicaciones Distribuidas S.L. 
 *  http://www.libelium.com 
 *  
 *  This program is free software: you can redistribute it and/or modify 
 *  it under the terms of the GNU General Public License as published by 
 *  the Free Software Foundation, either version 3 of the License, or 
 *  (at your option) any later version. 
 *  
 *  This program is distributed in the hope that it will be useful, 
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of 
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License 
 *  along with this program.  If not, see http://www.gnu.org/licenses/. 
 *  
 *  Version:           1.2
 *  Design:            David Gascón
 *  Implementation:    Covadonga Albiñana, Victor Boria, Ruben Martin
 *  
 *  Version 1.3
 *  Implementation: Charalampos Doukas, OpenIoT, CREATE-NET
 *  Description: Sample Arduino code for sampling from a Grove sensor
 *  and performing a broadcast transmition 
 *  This sample code reads from the analog port (A0) and transmits both on demand
 *  based on sensor changes and on every defined interval
 */

#include <Wire.h>

// Cooking API libraries
#include <arduinoUtils.h>

// Include the SX1272 and SPI library:
#include "arduinoLoRa.h"
#include <SPI.h>
#include "Arduino.h"
#include "DHT.h"
#include "Barometer.h"

//modify this to transmit based on an interval
//#define INTERVAL 300000   //trasnmit every X minutes (5 here as en example)
#define INTERVAL 10000   // test of transmission
#define DHTPIN A0         // what pin we're connected to for TEMPERATURE and HUMIDITY SENSOR
#define LIGHTPIN A1       // what pin we're connected to for LIGHT SENSOR
#define NOISEPIN A2    // what pin we're connected to for NOISE SENSOR
// Uncomment whatever type you're using!
//#define DHTTYPE DHT11   // DHT 11 
#define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

//UNIQUE ID for ARDUINO; modify this accordingly
String sensor_ID = "A4";

//modify to set the sensor value change threshold
//if the new sensor value differs for more then the
//defined threshold, a transmition will occur

int e;
char message1[60];

//modife for analog pin accordingly
float temperature;
float pressure;
float atm;
float altitude;
Barometer myBarometer;

String stringvalue = "";
unsigned long time;
unsigned long temp;

DHT dht(DHTPIN, DHTTYPE);

void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(9600);

  // Print a start message
  Serial.println(F("SX1272 module and Arduino: send packets without ACK"));
  
  powerLoRaOn();

  Serial.println(F("Temperature and Humidity sensors seetup"));
  dht.begin();
  Serial.println(F("Barometer sensors seetup"));
  myBarometer.init();

  temp = 0; 
}

void loop(void)
{


  time = millis();
  //check if it's time to transmit:
  if(time - temp > INTERVAL) {
    temp = time;

    //power LoRa ON:
    //powerLoRaOn();


// Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    // check if returns are valid, if they are NaN (not a number) then something went wrong!
    if (isnan(t) || isnan(h)) 
    {
        Serial.println("Failed to read from DHT");
    }

   //Reading temperature and pressure
   temperature = myBarometer.bmp085GetTemperature(myBarometer.bmp085ReadUT()); //Get the temperature, bmp085ReadUT MUST be called first
   pressure = myBarometer.bmp085GetPressure(myBarometer.bmp085ReadUP());//Get the temperature
   altitude = myBarometer.calcAltitude(pressure); //Uncompensated calculation - in Meters 
   atm = pressure / 101325; 
  
   /*Serial.print("Temperature: ");
   Serial.print(temperature, 2); //display 2 decimal places
   Serial.println("deg C");
   Serial.print("Pressure: ");
   Serial.print(pressure, 0); //whole number only.
   Serial.println(" Pa");
   Serial.print("Ralated Atmosphere: ");
   Serial.println(atm, 4); //display 4 decimal places
   Serial.print("Altitude: ");
   Serial.print(altitude, 2); //display 2 decimal places
   Serial.println(" m");
   Serial.println();*/
   
    //Reading light from LIGHTPIN
    Serial.print(F("-----------------\n"));
    int lightValue = analogRead(LIGHTPIN); 
    //Serial.print("Light: ");
    //Serial.print(lightValue);
    //Serial.println();

    //Reading noise from NOPISEPIN
    int noiseValue = analogRead(NOISEPIN); 
    //Serial.print("Noise: ");
    //Serial.print(noiseValue);
    //Serial.println();

    //Creation of LoRa Message
    stringvalue = sensor_ID;
    stringvalue += "|";
    stringvalue += "H=";
    stringvalue += h;
    //Temperature
    stringvalue += "|";
    stringvalue += "TA=";
    stringvalue += t;
    stringvalue += "|";
    stringvalue += "TB=";
    stringvalue += temperature;
    stringvalue += "|";
    stringvalue += "P=";
    stringvalue += pressure;
    stringvalue += "|";
    stringvalue += "L=";
    stringvalue += lightValue;
    stringvalue += "|";
    stringvalue += "N=";
    stringvalue += noiseValue;
    Serial.println(stringvalue);
    //Serial.println(stringvalue.length());
    //stringvalue.toCharArray(message1, 60);
    //stringvalue.toCharArray(message1, stringvalue.length()+1);
    Serial.println(message1);
    
    // Send (Broadcast) message1 and print the result
    Serial.print(F("Sending packet\n"));
    e = sx1272.sendPacketTimeout(0, message1);
    Serial.print(F("Packet sent, state "));
    Serial.println(e, DEC);

    //power LoRa OFF:
    //powerLoRaOff();
  
  }

  delay(2000);
}

void powerLoRaOn() {

  // Power ON the module
  e = sx1272.ON();
  Serial.print(F("Setting power ON: state "));
  Serial.println(e, DEC);
  
  // Set transmission mode and print the result
  e |= sx1272.setMode(4);
  Serial.print(F("Setting Mode: state "));
  Serial.println(e, DEC);
  
  // Set header
  e |= sx1272.setHeaderON();
  Serial.print(F("Setting Header ON: state "));
  Serial.println(e, DEC);
  
  // Select frequency channel
  e |= sx1272.setChannel(CH_10_868);
  Serial.print(F("Setting Channel: state "));
  Serial.println(e, DEC);
  
  // Set CRC
  e |= sx1272.setCRC_ON();
  Serial.print(F("Setting CRC ON: state "));
  Serial.println(e, DEC);
  
  // Select output power (Max, High or Low)
  e |= sx1272.setPower('H');
  Serial.print(F("Setting Power: state "));
  Serial.println(e, DEC);
  
  // Set the node address and print the result
  e |= sx1272.setNodeAddress(2);
  Serial.print(F("Setting node address: state "));
  Serial.println(e, DEC);
  
  // Print a success message
  if (e == 0)
    Serial.println(F("SX1272 successfully configured"));
  else
    Serial.println(F("SX1272 initialization failed"));

}

void powerLoRaOff() {
 sx1272.OFF();
}
