/*
  A simple program designed to setup and demonstrate the GY-30 (BH1750) sensor library and 
  the module - BDAA100002
 
  The program uses Claws BH1750 library to interact and monitor the output from the module
  through an I2C communication channel. On reading from the module it to outputs the result to 
  the serial monitor.
  
  modified 12th July 2019
  by Sebastian Karam - Flux Workshop
  
  The Adafruit Unified Sensor Driver library created by Claws
  https://github.com/claws/BH1750
*/ 
#include <WiFiNINA.h>
#include <Wire.h>  // include the standard Wire library
#include <BH1750.h> // include the BH1750 library
#include "secrets.h" 
BH1750 GY30; // instantiate a sensor event object

char ssid[] = SECRET_SSID;        
char pass[] = SECRET_PASS;
WiFiClient client;
char   HOST_NAME[] = "maker.ifttt.com";
String PATH_NAME   = "/trigger/light_level/with/key/c60HHHXkeOXUxmjCv95JOi"; // change your EVENT-NAME and YOUR-KEY

void setup(){
  // initialize WiFi connection
  WiFi.begin(ssid, pass);

  Serial.begin(9600);
  while (!Serial);

  // connect to web server on port 80:
  if (client.connect(HOST_NAME, 80)) {
    // if connected:
    Serial.println("Connected to server");
  }
  else {// if not connected:
    Serial.println("connection failed");
  }
  pinMode(13, OUTPUT); 
  digitalWrite(13, HIGH);  
  Serial.begin(9600);
  Wire.begin();
  GY30.begin();
}
void loop() {
  // Read the light level from the sensor
  float lux = GY30.readLightLevel();
  Serial.println();
  Serial.println((String)lux + " lx");
  Serial.println();
  String queryString = "?value1=" + String(lux);

  // Make an HTTP request to the webhook
  if (client.connect(HOST_NAME, 80)) {
    client.println("GET " + PATH_NAME + queryString + " HTTP/1.1");
    client.println("Host: " + String(HOST_NAME));
    client.println("Connection: close");
    client.println(); // end HTTP header

    while (client.connected()) {
      if (client.available()) {
        // Read an incoming byte from the server and print it to the serial monitor
        char c = client.read();
        Serial.print(c);
      }
    }
    client.stop(); // Close the connection
  } else {
    Serial.println("Failed to connect to server");
  }

  // Pause for 60 seconds before repeating the sensor poll
  delay(60000);
}

