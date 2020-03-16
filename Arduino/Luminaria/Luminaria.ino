#include <ArduinoJson.h>

/*
  UDPSendReceive.pde:
  This sketch receives UDP message strings, prints them to the serial port
  and sends an "acknowledge" string back to the sender

  A Processing sketch is included at the end of file that can be used to send
  and received messages for testing with a computer.

  created 21 Aug 2010
  by Michael Margolis

  This code is in the public domain.

  adapted from Ethernet library examples
*/

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#ifndef STASSID
#define STASSID "your-ssid"
#define STAPSK  "your-password"
#endif

unsigned int localPort = 8888; 
unsigned int remotePort = 8888;  // local port to listen on

IPAddress remoteIp(192, 168, 0, 1); 
// buffers for receiving and sending data
char packetBuffer[UDP_TX_PACKET_MAX_SIZE + 1]; //buffer to hold incoming packet,
char  ReplyBuffer[] = "acknowledged\r\n";       // a string to send back

WiFiUDP Udp;

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(STASSID, STAPSK);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(500);
  }
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());
  Serial.printf("UDP server on port %d\n", localPort);
  Udp.begin(localPort);
}
float measureAmps(){
  float sample=0;
  for(int i=0 ; i<150 ; i++){
    sample += takeAmpSample();
    delay(2);
    }
  float ret = sample/150;
  
  return ret;
  }

float takeAmpSample(){
  float sample = analogRead(A0); //read the current from sensor
  float x = mapfloat(sample, 0 , 1023 ,0 ,5 );
  float a= (10.0 * x);
  float y =  a - 25.0 ;
  if ( y < 0){
    y = y*(-1);
    }
  return y;
  }
  
float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void loop() {

  StaticJsonDocument<100> json;
  
  JsonArray values = json.createNestedArray("Values");
  values.add(measureAmps());

  // Log
  Serial.print(F("Sending to "));
  Serial.print(remoteIp);
  Serial.print(F(" on port "));
  Serial.println(remotePort);
  serializeJson(json, Serial);

  // Send UDP packet
  Udp.beginPacket(remoteIp, remotePort);
  serializeJson(json, Udp);
  Udp.println();
  Udp.write(ReplyBuffer);
  Udp.endPacket();

  delay(5000);

}
