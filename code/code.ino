#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include "max6675.h"
#include <Wire.h>
LiquidCrystal_I2C lcd(0x3F, 16, 2);

int thermoDO = 12;
int thermoCS = 15;
int thermoCLK = 14;
MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);
 
String apiKey = "MM2Q0KY7ZERFYF5V"; // Enter your Write API key from ThingSpeak
const char *ssid = "projetos_auto";     // replace with your wifi ssid and wpa2 key
const char *pass = "CybertronIFM23";
const char* server = "api.thingspeak.com";
 
 
WiFiClient client;
 


void setup() {
  Serial.begin(9600);
  Serial.println("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
    Serial.println("");
    Serial.println("WiFi connected");

    lcd.init();
    lcd.clear();         
    lcd.backlight();      // Make sure backlight is on
    
    // Print a message on both lines of the LCD.

}
 
void loop() 
{
      int celsius=thermocouple.readCelsius();
      lcd.setCursor(2,0);   //Set cursor to character 2 on line 0
      lcd.print("Temperatura ");
      
    
     lcd.setCursor(7,1);   //Set cursor to character 2 on line 0
     lcd.print(celsius);
     lcd.print("c"); 
     
     Serial.print("C = "); 
     Serial.println(celsius);
     Serial.print(" º "); 
 
    if (client.connect(server, 80)) // "184.106.153.149" or api.thingspeak.com
  {
    String postStr = apiKey;
    postStr += "&field1=";
    postStr += String(celsius);
    postStr += "r\n";
    
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);
    
    Serial.println("Data Send to Thingspeak");
  }
    client.stop();
    Serial.println("Waiting...");
 
    delay(2000);      // thingspeak needs minimum 15 sec delay between updates.
}
