#include <ESP8266WiFi.h>

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
    Serial.println("No networks found");
   }else{
    for(int i=0;i<n;++i){
      Serial.print(WiFi.SSID(i));
      Serial.print(WiFi.RSSI(i));
      Serial.print(",");
    }
  } 
}
