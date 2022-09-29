#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

char ssid[] = "test";    // Nome da rede
char password[] = "123456789"; // Senha de rede

IPAddress ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

unsigned int localUdpPort = 4211; 
char incomingPacket[255];  // buffer for incoming packets
char ReplyBuffer[] = "Broadcast";
IPAddress Broadcast(192,168,1,255);

IPAddress noAncora1(192,168,1,2);
IPAddress noAncora2(192,168,1,3);
IPAddress noAncora3(192,168,1,4);

//char* stringAncora1[100];
//char* stringAncora2[100];

float somaAncora1, somaAncora2, somaAncora3 = 0;
float mediaAncora1,mediaAncora2,mediaAncora3 = 0;

float minMax_x = 0;
float minMax_y = 0;

float distancia[3];

float xPosicao[3] = {0.5 ,0.0 ,-0.5};
float yPosicao[3] = {0.0 ,0.5 ,0.0};

int a1,a2,a3=0;

double distance;
float N = 3.4907;
float A = 63;

//0,5m ; 2m ; 4m
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

    //Serial.println("Contents:"); //0.5m,2m,4m
    //Serial.println(incomingPacket);

    if(remoteIp == noAncora1){ // verificação se o IP recebido é igual o nó esperado
      if(a1<100){
        Serial.println("Ancora1 :");
        somaAncora1 = somaAncora1 + String(incomingPacket).toInt(); // Realizar soma dos valores de RSSI para calculo de média
        Serial.println(somaAncora1);
        Serial.println(a1);
        a1++;
      }
    }else if(remoteIp == noAncora2){
      if(a2<100){
      Serial.println("Ancora2: ");
      somaAncora2 = somaAncora2 + String(incomingPacket).toInt();// Realizar soma dos valores de RSSI para calculo de média
      Serial.println(somaAncora2);
      Serial.println(a2);
      a2++;
      } 
    }else if(remoteIp == noAncora3){
      if(a3<100){
      Serial.println("Ancora3: ");
      somaAncora3 = somaAncora3 + String(incomingPacket).toInt();// Realizar soma dos valores de RSSI para calculo de média
      Serial.println(somaAncora3);
      Serial.println(a3);
      a3++;
      }   
    }
  
  // send a reply, to the IP address and port that sent us the packet we received
  delay(300);
  broadcast();
  }
  if(a1==100 && a2==100 && a3==100){ 
    mediaAncora1 = somaAncora1/100;
    mediaAncora2 = somaAncora2/100;
    mediaAncora2 = somaAncora3/100;
    Serial.printf("Média Ancora 1: %f\nMédia Ancora 2: %f\nMédia Ancora 3: %f\n", mediaAncora1,mediaAncora2,mediaAncora3);
    distancia[0] = calcDistance(mediaAncora1);
    distancia[1] = calcDistance(mediaAncora2);
    distancia[2] = calcDistance(mediaAncora3);
    
    minMax();
    a1,a2,a3 = 0;
    somaAncora1,somaAncora2,somaAncora3=0;
  }
}

void broadcast(){
  if(Udp.beginPacket(Broadcast, localUdpPort) == 1){
    Udp.write(ReplyBuffer);
    Serial.println(ReplyBuffer);
    Udp.endPacket();
    };
}

float calcDistance(long rssi){
//RSSI = A - 10 * n * log( d )
//d = 10^((A-RSSI)/10*n)
float ratio = (rssi - A)/(10 * N);
distance = pow(10,ratio);
Serial.print(distance);
return distance;
}

void minMax(){
  float max_x, min_x, max_y, min_y;

  for(int i = 0; i < 3; i++){
    if(i==0){
      
      max_x = xPosicao[i] - distancia[i];
      min_x = xPosicao[i] + distancia[i];

      max_y = yPosicao[i] - distancia[i];
      min_y = yPosicao[i] + distancia[i]; 
      
    }else{
      
      if(max_x <  xPosicao[i] - distancia[i]){
        max_x = xPosicao[i] - distancia[i];  
      }
      if(min_x >  xPosicao[i] + distancia[i]){
        min_x = xPosicao[i] + distancia[i];  
      }
      if(max_y <  yPosicao[i] - distancia[i]){
        max_y = yPosicao[i] - distancia[i];  
      }
      if(min_y >  yPosicao[i] + distancia[i]){
        min_y = yPosicao[i] + distancia[i];  
      }  
    } 
  }
Serial.println("Valores");
Serial.println(max_x);
Serial.println(min_x);
Serial.println(max_y);
Serial.println(min_y);  
minMax_x = (max_x + min_x)/2;
minMax_y = (max_y + min_y)/2;
Serial.println("X e Y");
Serial.println(minMax_x);
Serial.println(minMax_y);
}
