#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

char ssid[] = "test";    // Nome da rede
char password[] = "123456789"; // Senha de rede

IPAddress ip(192,168,1,1);
IPAddress gateway(192,168,1,0);
IPAddress subnet(255,255,255,0);

unsigned int localUdpPort = 4211; 
char incomingPacket[255];  // buffer for incoming packets
char ReplyBuffer1[] = "Broadcast";
char ReplyBuffer2[] = "A";
IPAddress Broadcast(192,168,1,255);

WiFiUDP Udp;

void setup()
{
  Serial.begin(115200);
  
  while (!Serial) {
    ;
  }

  Serial.print("Setting mode Wifi-AP ... ");
  WiFi.mode(WIFI_AP);
  
  Serial.print("Setting soft-AP connection... ");
  Serial.println(WiFi.softAP(ssid, password,1,false,8) ? "Ready" : "Failed!");
  
  Serial.println("Wait 200 ms to start...");
  delay(200);
  
  Serial.print("Setting soft-AP configuration ... ");
  Serial.println(WiFi.softAPConfig(ip, gateway, subnet) ? "Ready" : "Failed!");

  Serial.print("Soft-AP IP address = ");
  Serial.println(WiFi.softAPIP());

  Udp.begin(localUdpPort);
  Serial.printf("Now listening at UDP port %d\n", localUdpPort);
}

void loop() {
  int packetSize = Udp.parsePacket();
  
  if (packetSize) {

    Serial.print("Received packet of size ");

    Serial.println(packetSize);

    Serial.print("From ");

    IPAddress remoteIp = Udp.remoteIP();

    Serial.print(remoteIp);

    Serial.print(", port ");

    Serial.println(Udp.remotePort());

    // read the packet into packetBufffer

    int len = Udp.read(incomingPacket, 255);

    if (len > 0) {

      incomingPacket[len] = 0;

    }

    Serial.println("Contents:");

    Serial.println(incomingPacket);

    // send a reply, to the IP address and port that sent us the packet we received

  Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
  Udp.write(ReplyBuffer2);
  Udp.endPacket();

  }

  if(Udp.beginPacket(Broadcast, localUdpPort) == 1){
    Udp.write(ReplyBuffer1);
    Serial.println(ReplyBuffer1);
    Udp.endPacket();
    };
  
  delay(1000);
}
