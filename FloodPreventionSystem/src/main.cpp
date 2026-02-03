#include <Arduino.h>
#include <ESP8266WiFi.h>

// Prototypes
void wifi_connect();

void setup() {
  Serial.begin(9600);
  delay(1000);

  wifi_connect();


}

void loop() {
  // put your main code here, to run repeatedly:
}

// Basic 2.4GHz WiFi connection
void wifi_connect() {
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);

  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}