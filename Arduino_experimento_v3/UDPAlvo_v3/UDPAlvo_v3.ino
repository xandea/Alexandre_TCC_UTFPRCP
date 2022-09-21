#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

char ssid[] = "test";    // Nome da rede
char password[] = "123456789"; // Senha de rede

IPAddress ip(192,168,1,1);
IPAddress gateway(192,168,1,0);
IPAddress subnet(255,255,255,0);

unsigned int localUdpPort = 4211; 
char incomingPacket[255];  // buffer for incoming packets
char ReplyBuffer[] = "Broadcast";
IPAddress Broadcast(192,168,1,255);

IPAddress noAncora1(192,168,1,2);
IPAddress Ancora2(192,168,1,3);

char* stringAncora1[100];
char* stringAncora2[100];

int a1=0;
int a2=0;

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

  while(WiFi.softAPgetStationNum()==0){
    delay(5000);
    Serial.println("Not Connected");
  }
  Serial.println("Station Connected");
  delay(5000);
  broadcast();
}

void loop() {
  int packetSize = Udp.parsePacket();
  
  if (packetSize) {
   
    //Serial.print("Received packet of size ");

    //Serial.println(packetSize);

    //Serial.print("From ");

    IPAddress remoteIp = Udp.remoteIP();

    //Serial.print(remoteIp);

    //Serial.print(", port ");

    //Serial.println(Udp.remotePort());

    // read the packet into packetBufffer

    int len = Udp.read(incomingPacket, 255);

    if (len > 0) {

      incomingPacket[len] = 0;

    }

    //Serial.println("Contents:");
    //Serial.println(incomingPacket);

    if(remoteIp == noAncora1){
      Serial.println("Ancora1 :");
      stringAncora1[a1] = incomingPacket;
      Serial.println(stringAncora1[a1]);
      a1++;
    }else{
      Serial.println("Ancora2: ");
      stringAncora2[a2] = incomingPacket;
      Serial.println(stringAncora2[a2]);
      a2++;   
    }

  /*if(a1 == 10){
    for(int j=0; j<=a1;j++){
    Serial.println(stringAncora1[j]);  
    }  
  }*/

  // send a reply, to the IP address and port that sent us the packet we received
  delay(1000);
  broadcast();
  }
}

void broadcast(){
  if(Udp.beginPacket(Broadcast, localUdpPort) == 1){
    Udp.write(ReplyBuffer);
    Serial.println(ReplyBuffer);
    Udp.endPacket();
    };
}
