#include "WifiSwitcher.h"
#include "SocketIoClient.h"
#include <ESP8266HTTPClient.h>

WifiSwitcher *wifiSwitcher;

SocketIoClient socket;

const char* host = "http://raspbespi.local";
const char* url = "/";
const int port = 3000;

boolean pulledDown = false;

void setup()
{
  //  pinMode(LED_BUILTIN, OUTPUT);
  //  wifiSwitcher = new WifiSwitcher();
  //  if (wifiSwitcher->launchHotSpot("ESP12Emeric", "0671755362")){
  //      digitalWrite(LED_BUILTIN, LOW);
  //      pinMode(D7, INPUT);
  //  } else {
  //       blinkError();
  //  }

  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  WiFi.begin("SFR_4242", "0671755362");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
    digitalWrite(LED_BUILTIN, HIGH);
  }

  if (WiFi.status() == WL_CONNECTED) {
    digitalWrite(LED_BUILTIN, LOW);
    pinMode(D7, INPUT);

    socket.on("connect", handler);
    socket.on("disconnected", diconnectedHandler);
    socket.begin("192.168.1.20", 3000);
    
  } else {
    digitalWrite(LED_BUILTIN, HIGH);
    ESP.deepSleep(0);
  }
}

void loop()
{
  //  switch (wifiSwitcher->state){
  //    case STATION:
  //      wifiSwitcher->waitingForInternetWifiConnection();
  //      break;
  //    case LINKED:
  //      buttonGesture();
  //      break;
  //    case DOWN:
  //      delete wifiSwitcher;
  //      blinkError();
  //      break;
  //  }
  buttonGesture();
}

void buttonGesture() {
  Serial.println("Waiting for Switch !");
  if (digitalRead(D7) == HIGH && !pulledDown) {
    pulledDown = true;
    socket.emit("switch lamp", "1");
  } else {
    digitalWrite(LED_BUILTIN, HIGH);
  }

  if (digitalRead(D7) == LOW){
    pulledDown = false;
  }
  socket.loop();
  delay(500);
}

void handler(const char * payload, size_t length) {
  Serial.print("Connected !");
  digitalWrite(LED_BUILTIN, LOW);
}

void diconnectedHandler(const char * payload, size_t length) {
  Serial.print("disconnected !");
  digitalWrite(LED_BUILTIN, HIGH);
  delay(5000);
  socket.begin("192.168.1.20", 3000);
}
