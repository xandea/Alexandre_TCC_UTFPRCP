// Code to get header here: https://carvesystems.com/news/writing-a-simple-esp8266-based-sniffer/
// 0,000000005 s should be the measured time for 1m. For sending and receiving it should be 0,00000001 s = 0,00001 ms = 0,01 micros 
// For cycle counts with 160 MHz it should be 1 s          --- 160 * 1000*1000
//                                            0,01 * 10^-6 --- x
//                                                x = 160*0,01 = 1,6

// Resultados importantes que valeria a pena explorar: https://people.csail.mit.edu/bkph/ftmrtt

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const char* ssid = "NodeMCUNet";
const char* password = "cachorro1";

WiFiUDP Udp;
unsigned int localUdpPort = 4220;  // local port to listen on
unsigned int sendUdpPort = 4210;  // port of the receiver 
char incomingPacket[255];  // buffer for incoming packets
char sendPacket[] = "S";  // a byte to start measuring
char  replyPacket[] = "A";  // a reply string to send back
IPAddress receiverIP;
unsigned long time_before_message;
unsigned long time_after_message;
unsigned long time_aux;
boolean receiver_up = 0;
boolean flag_sent = 0;

void setup()
{
  Serial.begin(115200);
  Serial.println("Iniciando Setup");
  WiFi.mode(WIFI_AP);
  
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  
  Udp.begin(localUdpPort);
  Serial.printf("Now starting to send and listening at IP %s, UDP port %d\n", myIP.toString().c_str(), localUdpPort);
}


void loop()
{
  if (!receiver_up) {
    wait_receiver();
    delay(200);
  }
  else {
    if (!flag_sent) {
      Udp.beginPacket(receiverIP, sendUdpPort);
      Udp.write(sendPacket);
      Udp.endPacket();
      time_before_message = ESP.getCycleCount();
      //Serial.printf("Message sent with %d bytes to %s, port %d\n", strlen(sendPacket), sendIP, sendUdpPort);
      flag_sent = 1;
    }
  //  delay(1000);
    
    int packetSize = Udp.parsePacket();
    time_after_message = ESP.getCycleCount();
    if (packetSize)
    {
      //Serial.println(time_after_message-time_before_message);
      // receive incoming UDP packets
      //Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
      int len = Udp.read(incomingPacket, 255);
      if (len > 0)
      {
        incomingPacket[len] = 0;
      }
      //Serial.print(incomingPacket);
     
      Serial.printf("D%s\n", incomingPacket);
      
      
      /*int n = WiFi.scanNetworks();
      if(n==0){
        Serial.println("No networks found");
      }else{
        for(int i=0;i<n;++i){
          Serial.print(WiFi.SSID(i));
          Serial.print(WiFi.RSSI(i));
          Serial.print(",");
        }
      } */
      //Serial.println(WiFi.RSSI());
      // send back a reply, to the IP address and port we got the packet from
      Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
      Udp.write(replyPacket);
      Udp.endPacket();
      flag_sent = 0;
    }
  }
}

void wait_receiver() {
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
    receiverIP = Udp.remoteIP();
    int len = Udp.read(incomingPacket, 255);
    if (len > 0)
    {
      incomingPacket[len] = 0;
    }
    Serial.printf("UDP packet contents: %s\n", incomingPacket);
    receiver_up = 1;
    // send back a reply, to the IP address and port we got the packet from
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.write(replyPacket);
    Udp.endPacket();
  }
}
