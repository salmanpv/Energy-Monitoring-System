// EmonLibrary examples openenergymonitor.org, Licence GNU GPL V3
#include "EmonLib.h"             // Include Emon Library
#include <WiFi.h>
#include <WiFiClient.h>
#include <ThingSpeak.h>

EnergyMonitor emon;             // Create an instance
WiFiClient client;

char ssid[] = "your wifi ssid";
char pass[] = "your password";
unsigned long myChannelNumber = 2000007; //Your Channel Number (Without Brackets)
const char * myWriteAPIKey = "9XXXXXXXXXXXXXXI"; //Your Write API Key
float v,i,p,e;
float kWh = 0;
unsigned long lastmillis = millis();

void setup()
{  
  Serial.begin(9600);
  delay(10);
  WiFi.begin(ssid, pass);
  ThingSpeak.begin(client);
  vcal = 0;
  ical = 0;
  emon.voltage(35, vcal, 1.7);  // Voltage: input pin, calibration, phase_shift
  emon.current(34, ical);       // Current: input pin, calibration.
}

void loop()
{
  emon.calcVI(20,2000);         // Calculate all. No.of half wavelengths (crossings), time-out
  v = emon.Vrms;
  i = emon.Irms;
  p = emon.apparentPower;
  Serial.print("Vrms: ");
  Serial.print(v, 2);
  Serial.print("V");
  Serial.print("\tIrms: ");
  Serial.print(i, 4);
  Serial.print("A");
  Serial.print("\tPower: ");
  Serial.print(p, 4);
  Serial.print("W");
  Serial.print("\tkWh: ");
  kWh = kWh + p*(millis()-lastmillis)/3600000000.0;
  e = kWh;
  Serial.print(kWh, 4);
  Serial.println("kWh");
  lastmillis = millis();
  delay(1000);
  ThingSpeak.writeField(myChannelNumber, 1,v, myWriteAPIKey);
  ThingSpeak.writeField(myChannelNumber, 2,i, myWriteAPIKey);
  ThingSpeak.writeField(myChannelNumber, 3,p, myWriteAPIKey);
  ThingSpeak.writeField(myChannelNumber, 4,e, myWriteAPIKey);
  delay(100);
}
