
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include "./WIFIsettings.cpp"
#include <ArduinoOTA.h> // OTA Upload via ArduinoIDE


#include <Adafruit_NeoPixel.h>
#define PIN 2

#define numLED 100
Adafruit_NeoPixel strip = Adafruit_NeoPixel(numLED, PIN, NEO_GRB + NEO_KHZ800);


/* Set these to your desired credentials. */


ESP8266WebServer server(80);

/* Just a little test message.  Go to http://192.168.4.1 in a web browser
   connected to this access point to see it.
*/
void handleRoot()
{
    server.send(200, "text/html", "Nothing here. Please connect to /LED");
}
void handleLED()
{
    Serial.println("handle Root");
    String message="";
    for(int i=0; i<numLED; i++){
        u_int32_t color=strip.getPixelColor(i);

       //message+=iToString(color,10)+"|";
       message+=iToHex(color,6)+"|";

        //Serial.println(message);
    }
    server.send(200, "text/html", message);
}
void handleNotFound()
{
    server.send(200, "text/html", "<h1>NotFound heheee</h1>");
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

    server.on("/",handleRoot);
    server.on("/LED", handleLED);
    server.onNotFound(handleNotFound);
    server.begin();
    Serial.println("HTTP server started");

    ArduinoOTA.begin(); // OTA Upload via ArduinoIDE
    ArduinoOTA.setHostname("esp8266-01_LED_MASTER");

    //Serial.println("DebugMessage1");
    strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
    strip.show();            // Turn OFF all pixels ASAP
    //Serial.println("DebugMessage2");
    strip.fill(strip.Color(0,255,0),0,numLED);

    Serial.println("finshed setup");
}

void loop()
{
    server.handleClient();
    
    ArduinoOTA.handle(); // OTA Upload via ArduinoIDE
}

String iToString(u_int32_t value, int TargetLength){

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

String iToHex(u_int32_t value, int TargetLength){
    String result;
    String valueString=String(value,HEX);
    for (int i=valueString.length(); i<TargetLength;i++){
        result += String("0");
    }
    result+=valueString;
    return result;
}

