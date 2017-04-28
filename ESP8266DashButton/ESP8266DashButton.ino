#include "WifiSwitcher.h"
#include "SocketIoClient.h"
#include <ESP8266HTTPClient.h>

WifiSwitcher *wifiSwitcher;

boolean pulledDown = false;

void setup()
{
    Serial.begin(115200);
    pinMode(LED_BUILTIN, OUTPUT);
    wifiSwitcher = new WifiSwitcher();
    if (wifiSwitcher->launchHotSpot("ESP12Emeric", "0671755362")){
        digitalWrite(LED_BUILTIN, LOW);
        pinMode(D7, INPUT);
    } else {
         blinkError();
    }
}

void loop()
{
    switch (wifiSwitcher->state){
      case STATION:
        wifiSwitcher->waitingForInternetWifiConnection();
        break;
      case LINKED:
        buttonGesture();
        break;
      case DOWN:
        delete wifiSwitcher;
        blinkError();
        break;
    }
}

void buttonGesture() {
  if (digitalRead(D7) == HIGH && !pulledDown) {
    pulledDown = true;
    if (wifiSwitcher->webhook != NULL){ 
      HTTPClient http;
      http.begin(*wifiSwitcher->webhook);

      int httpCode = http.GET();

      if (httpCode > 0) { //Check the returning code
        String payload = http.getString();   //Get the request response payload
        Serial.println(payload);                     //Print the response payload
        Serial.print("WebHook :");Serial.println(*wifiSwitcher->webhook);
      } else {
        Serial.print("WebHook :");Serial.println("Error sending request.");
      }

      http.end();
    } else {
      Serial.println("Unknown Webhook");
    }
  } else {
    digitalWrite(LED_BUILTIN, HIGH);
  }

  if (digitalRead(D7) == LOW){
    pulledDown = false;
    //ESP.deepSleep(0);
  }
}

void blinkError(){
  Serial.println("Error !");
  while (1){
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
  }
}

