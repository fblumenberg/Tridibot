#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WebSocketsServer.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include "FS.h"

//OTA Stuff
#include <ESP8266HTTPUpdateServer.h>
ESP8266HTTPUpdateServer httpUpdater;

//Uncomment to debug
//#define debug

int clientCont = 0;
long previousMillis = 0;
String com = "S";
char output = 'S';
char input = 'S';
boolean Ready = true;
int clientID;
boolean first = true;

String botName = "none";
String actSpeed = "2";
String actColor = "4";
String motorStatus = "Y";

boolean progMode = false;

ESP8266WiFiMulti WiFiMulti;

IPAddress apIP(192, 168, 1, 1);

ESP8266WebServer server (80);
WebSocketsServer webSocket = WebSocketsServer(81);


void sendCommand() {
  if (com.equals("S")) {
    output = 'S';
  } else if (com.equals("F")) {
    output = 'F';
  } else if (com.equals("L")) {
    output = 'L';
  } else if (com.equals("R")) {
    output = 'R';
  } else if (com.equals("B")) {
    output = 'B';
  } else if (com.equals("A")) {
    output = 'A';
  } else if (com.equals("E")) {
    output = 'E';
  } else if (com.equals("I")) {
    output = 'I';
  } else if (com.equals("O")) {
    output = 'O';
  } else if (com.startsWith("C")) {
    Serial.write('C');
    Serial.write(com[1]);
    actColor = com[1];

    String info = "nom" + botName;
    info += " vel" + actSpeed;
    info += " col" + actColor;
    info += " mot" + motorStatus;

    webSocket.broadcastTXT(info);
  } else if (com.startsWith("V")) {
    for (int i = 0; i < com.length(); i++) {
      Serial.write(com[i]);
    }
    actSpeed = com[1];

    String info = "nom" + botName;
    info += " vel" + actSpeed;
    info += " col" + actColor;
    info += " mot" + motorStatus;

    webSocket.broadcastTXT(info);
  } else if (com.equals("Y")) {
    motorStatus = "Y";
    output = 'Y';

    String info = "nom" + botName;
    info += " vel" + actSpeed;
    info += " col" + actColor;
    info += " mot" + motorStatus;
    webSocket.broadcastTXT(info);

  } else if (com.equals("Z")) {
    motorStatus = "Z";
    output = 'Z';

    String info = "nom" + botName;
    info += " vel" + actSpeed;
    info += " col" + actColor;
    info += " mot" + motorStatus;
    webSocket.broadcastTXT(info);

  } else {
    output = 'S';
  }
}

void setup() {
  Serial.begin(115200);
  delay(100);

  SPIFFS.begin();
  delay(100);

  connectionConfig();
  delay(100);

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  delay(100);

  otaInit();

  handleServer();
  delay(100);


}


void loop() {
  webSocket.loop();
  server.handleClient();
  sendCommand();

  if (output == 'S' && Ready && first) {
    Serial.write(output);
    Ready = false;
    first = false;
  } else if ( output != 'S' && Ready) {
    Serial.write(output);
    Ready = false;
    first = true;
  }

  if (Serial.available()) {
    while (Serial.available()) {
      Serial.read();
    }

    if (progMode) {
      webSocket.sendTXT(clientID, "n");
      progMode = false;
    }

    Ready = true;

  }

}
