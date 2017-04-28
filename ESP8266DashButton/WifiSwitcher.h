#ifndef  __WIFISWITCHER__
#define __WIFISWITCHER__

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

enum e_wifiState {
          STATION = 1,
          LINKED = 2,
          DOWN = 0
};

class WifiSwitcher {

  public:
    // Attributs
    e_wifiState state = STATION;
    String *webhook = NULL;
  
    // Constructor
    WifiSwitcher();
    WifiSwitcher (const WifiSwitcher&);
    // Destructor
    ~WifiSwitcher();

   /// HotSpot Methods
   boolean launchHotSpot(char* ssid, char* password);
   int howManyDevices();

   /// Web Service Methods
   boolean waitingForInternetWifiConnection();

 private:
    String ssid = "ESP12Emeric";
    String password = "06717554362";
    String *errorMessage = NULL;
    String *successMessage = NULL;
    ESP8266WebServer *server;

    /// HTTP Page Methods
   void initHTTPRoutes();

 protected:
   void getHTTPIndexPage();
   void getHTTPNotFound();
   void postHTTPInternetWifi();
};

#endif __WIFISTATION__
