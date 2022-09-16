#include <ESP8266WiFi.h>

const char* ssid = "teste";

void setup()
{
  Serial.begin(9600);
  Serial.println();
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(500);
  Serial.println("Setup done");
}

void loop() {
   int n = WiFi.scanNetworks();
   if(n==0){
    Serial.println("No networks found\n");
   }else{
    for(int i=0;i<n;++i){
      if(strcmp (ssid,WiFi.SSID(i).c_str()) == 0){
        Serial.printf("%s,%d\n", WiFi.SSID(i).c_str(), WiFi.RSSI(i));
      }
    }
  } 
}
