#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <WiFiClient.h>
#include "./WIFIsettings.cpp"
//#include <ArduinoOTA.h> // OTA Upload via ArduinoIDE


#include <Adafruit_NeoPixel.h>
#define PIN 2

#define numLED 100
Adafruit_NeoPixel strip = Adafruit_NeoPixel(numLED, PIN, NEO_GRB + NEO_KHZ800);

unsigned long previousTime=0;

/* Set these to your desired credentials. */

AsyncWebServer server(80);


void handle_Root(AsyncWebServerRequest *request)
{
    Serial.println("handle Root");
    //server.send(200, "text/html", "Nothing here. Please connect to <a href='/LED'>/LED</a>");
    request->send_P(200, "text/html", "Nothing here. Please connect to <a href='/LED'>/LED</a>");
}
void handle_LED(AsyncWebServerRequest *request)
{
    Serial.println("handle LED");
    String message="";
    //message+=iToString(numLED,4)+"|";
    Serial.println(iToHex(strip.getPixelColor(0),6));
    for(int i=0; i<numLED; i++){
        uint32_t color=strip.getPixelColor(i);

       //message+=iToString(color,10)+"|";
       message+=iToHex(color,6)+"";

        //Serial.println(message);
    }
    //server.send(200, "text/html", message);
    request->send(200, "text/html", message);
}
void handle_numLED(AsyncWebServerRequest *request)
{
    Serial.println("handle numLED");
    //server.send(200, "text/html", iToString(numLED, 4));
    request->send(200, "text/html", iToString(numLED, 4));
}

void handle_NotFound(AsyncWebServerRequest *request)
{
    Serial.println("handle NotFound");
    //server.send(404, "text/html", "<h1>NotFound heheee</h1>");
    request->send_P(404, "text/html", "<h1>NotFound</h1>");
}

void setup()
{
    delay(1000);
    
    Serial.begin(115200);

    WiFi.mode(WIFI_STA);
    WiFi.begin(HOMEssid, HOMEpsk);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("My IP-Adress is:  ");
    Serial.println(WiFi.localIP());

    Serial.println();
    Serial.print("Configuring access point...");
    /* You can remove the password parameter if you want the AP to be open. */
    WiFi.softAP(APssid, APpsk);

    IPAddress myIP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(myIP);

    SPIFFS.begin();

    server.on("/",handle_Root);
    server.on("/LED", handle_LED);
    server.on("/numLED", handle_numLED);
    server.onNotFound(handle_NotFound);
    server.begin();
    Serial.println("HTTP server started");

    // ArduinoOTA.begin(); // OTA Upload via ArduinoIDE
    // ArduinoOTA.setHostname("esp8266-01_LED_MASTER");

    //Serial.println("DebugMessage1");
    strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
    strip.show();            // Turn OFF all pixels ASAP
    //Serial.println("DebugMessage2");
    strip.fill(strip.Color(0,255,0),0,numLED);

    Serial.println("finshed setup");
}

int i = 0;
int dir=1;
void loop()
{
    //server.handleClient();
    //ArduinoOTA.handle(); // OTA Upload via ArduinoIDE

    if (millis()- previousTime>10){
        previousTime=millis();
        i+=dir;
        if(i==150||i==0){dir=-dir;}
        strip.fill(strip.Color(i, 0, 0), 0, numLED);
        strip.show();
    }
}

String iToString(uint32_t value, int TargetLength){

    String result;
    String valueString=(String) value;
    int lengthString=valueString.length();
    //int lengthString=sizeof(valueString)/sizeof(valueString[0]);
    // Serial.println(valueString);
    // Serial.println(valueString.length());
    // Serial.println(valueString[0]);
    // Serial.println(sizeof(valueString[0]));
    // Serial.println(lengthString);
    for (int i = 0;i<(TargetLength-lengthString);i++){
        result+=String("0");
    }
    
    result+=valueString;
    Serial.println(result);
    return result;

}

String iToHex(uint32_t value, int TargetLength){
    String result;
    String valueString=String(value,HEX);
    for (int i=valueString.length(); i<TargetLength;i++){
        result += String("0");
    }
    result+=valueString;
    return result;
}
