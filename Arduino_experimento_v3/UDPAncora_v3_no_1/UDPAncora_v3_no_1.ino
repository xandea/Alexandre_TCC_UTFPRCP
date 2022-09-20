#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

char ssid[] = "test";    // Nome da rede
char password[] = "123456789"; // Senha de rede

IPAddress ip(192,168,1,2);
IPAddress gateway(192,168,1,0);
IPAddress subnet(255,255,255,0);

unsigned int localUdpPort = 4211; 
char incomingPacket[255];  // buffer for incoming packets
char ReplyBuffer[] = "Reply";

WiFiUDP Udp;

void setup()
{
  Serial.begin(115200);
  
  while (!Serial) {
    ;
  }
  
  WiFi.mode(WIFI_STA);
  
  Serial.print("Setting STATION configuration ... ");
  Serial.println(WiFi.config(ip, gateway, subnet) ? "Ready" : "Failed!");
  
  while (WiFi.status() != WL_CONNECTED) {

    Serial.print("Attempting to connect to open SSID: ");

    Serial.println(ssid);

    WiFi.begin(ssid,password);

    // wait 5 seconds for connection:

    delay(5000);
  }
  
  Udp.begin(localUdpPort);
  Serial.printf("Connected, IP address: %s, UDP port %d\n", WiFi.localIP().toString().c_str(), localUdpPort);
  
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
  Udp.write("TESTE");
  Udp.endPacket();
  }
}
