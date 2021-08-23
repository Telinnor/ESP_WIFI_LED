
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include "./WIFIsettings.cpp"
#include <ArduinoOTA.h> // OTA Upload via ArduinoIDE




void setup()
{
    delay(1000);
    
    Serial.begin(115200);

    WiFi.mode(WIFI_STA);
    WiFi.begin(APssid, APpsk);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.print("My IP-Adress is:  ");
    Serial.println(WiFi.localIP());


    ArduinoOTA.begin(); // OTA Upload via ArduinoIDE
    ArduinoOTA.setHostname("esp8266-01_LED_CLIENT");

}

void loop()
{
    ArduinoOTA.handle(); // OTA Upload via ArduinoIDE
}