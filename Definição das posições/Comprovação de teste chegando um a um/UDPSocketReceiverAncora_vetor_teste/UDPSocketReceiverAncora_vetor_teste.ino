#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <iostream>
#include <string>

const char* ssid = "NodeMCUNet";
const char* password = "cachorro1";

WiFiUDP Udp;
unsigned int localUdpPort = 4210;  // local port to listen on
unsigned int sendUdpPort = 4220;  // port of the sender
char incomingPacket[255];  // buffer for incoming packets
char sendPacket[] = "Receiver up!";  // a reply string to send back
char  replyPacket[] = "A";  // a byte for signal measurements
char sendIP[] = "192.168.4.1";
boolean sender_up = 0;
long rssi;
long j=0;

void setup()
{
  Serial.begin(115200);
  Serial.println();

  Serial.printf("Connecting to %s \n", ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" connected");

  Udp.begin(localUdpPort);
  Serial.printf("Now listening at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), localUdpPort);
  
}


void loop()
{
  if (!sender_up) {
    wait_sender();
    delay(200);
  }
  else {
    int packetSize = Udp.parsePacket();
    if (packetSize)
    {
      // receive incoming UDP packets
      //Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
      int len = Udp.read(incomingPacket, 255);
      if (len > 0)
      {
        incomingPacket[len] = 0;
      }
      //Serial.printf("UDP packet contents: %s\n", incomingPacket);
      char temp[255];
      rssi = WiFi.RSSI();
      Serial.print("Rssi:");
      j = j+1;
      Serial.println(j);
      //Serial.println(rssi); teste
      rssi = abs(rssi);
      char buff[3];
      dtostrf(rssi,3,0,buff);
      // send back a reply, to the IP address and port we got the packet from
      Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
      char array[14]="abcdefghijklm";
      //Udp.write(buff); teste
      Udp.write(array[j]);
      Udp.endPacket();
    }
  //  Serial.println("Waiting for packet");
  //  delay(1000);
  }
}


void wait_sender() {
  Udp.beginPacket(sendIP, sendUdpPort);
  Udp.write(sendPacket);
  Udp.endPacket();
  
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
    int len = Udp.read(incomingPacket, 255);
    if (len > 0)
    {
      incomingPacket[len] = 0;
    }
    Serial.printf("UDP packet contents: %s\n", incomingPacket);
    sender_up = 1;
  }
}
