
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
//#include <ArduinoOTA.h> // OTA Upload via ArduinoIDE

#ifndef APSSID
#define APSSID "ESPap"
#define APPSK "thereisnospoon"
#endif

/* Set these to your desired credentials. */
const char *ssid = APSSID;
const char *password = APPSK;

ESP8266WebServer server(80);

/* Just a little test message.  Go to http://192.168.4.1 in a web browser
   connected to this access point to see it.
*/
void handleRoot()
{
    server.send(200, "text/html", "<h1>You are connected</h1>");
}
void handleNotFound()
{
    server.send(200, "text/html", "<h1>NotFound heheee</h1>");
}

void setup()
{
    delay(1000);
    //ArduinoOTA.begin(); // OTA Upload via ArduinoIDE
    Serial.begin(115200);

    WiFi.mode(WIFI_STA);
    WiFi.begin("connecting...", "382988400164138545921");

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println();
    Serial.print("Configuring access point...");
    /* You can remove the password parameter if you want the AP to be open. */
    WiFi.softAP(ssid, password);

    IPAddress myIP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(myIP);

    server.on("/", handleRoot);
    server.onNotFound(handleNotFound);
    server.begin();
    Serial.println("HTTP server started");
}

void loop()
{
    server.handleClient();
    //ArduinoOTA.handle(); // OTA Upload via ArduinoIDE
}