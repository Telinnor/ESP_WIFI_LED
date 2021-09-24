
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include "./WIFIsettings.cpp"

#include <Adafruit_NeoPixel.h>
#define PIN 2

#define numLED 100
Adafruit_NeoPixel strip = Adafruit_NeoPixel(numLED, PIN, NEO_GRB + NEO_KHZ800);

const char* host = "192.168.4.1";
const char* url = "/LED";
unsigned long previousTime = 0;

void setup()
{
  delay(1000);

  Serial.begin(9600);

  WiFi.mode(WIFI_STA);
  WiFi.begin(APssid, APpsk);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.print("My IP-Adress is:  ");
  Serial.println(WiFi.localIP());


  strip.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();

}

void loop()
{
  if (previousTime + 1000 < millis()) {
    previousTime = millis();
    clientAktion();
    strip.show();
  }

}

void clientAktion() {
  WiFiClient client;

  int versuche = 1;
  int erg;
  do  //mit dem Server eine Verbindung aufbauen
  {
    Serial.print("Verbindungsaufbau zu Server ");
    Serial.println(host);

    erg = client.connect(host, 80);
    if (!erg) {
      versuche++;
      Serial.println("Verbindungsaufbau nicht moeglich!!!");
      if (versuche > 3) {
        Serial.println("Klappt nicht, ich versuche es spaeter noch mal!");
        client.stop();
        WiFi.disconnect();
        ESP.deepSleep( 10 * 60000000); //Etwas später neu probieren, solange schlafen
      }
    }
    delay(1000);
  } while (erg != 1);


  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println("Timeout!");
      Serial.println("Uebergabe klappt nicht, ich versuche es spaeter noch mal!");
      client.stop();
      WiFi.disconnect();
      ESP.deepSleep( 60 * 1000000); //Etwas später 60 Sekunden neu probieren,solange schlafen
    }
  }

  Serial.print("Rueckgabe vom Server:\n");
  while (client.available()) {
    String line = client.readStringUntil('\r');
    //Serial.print(line);
    ReadLEDs(line);
  }

  client.stop();
}

void ReadLEDs(String line) {
  String LEDcount=""+ line.charAt(0) + line.charAt(1) + line.charAt(2) + line.charAt(3);
  Serial.println(LEDcount);
  for (int i = 0; i < LEDcount.toInt(); i++) {
    //char colorHEX[6] = {line.charAt(5 + i * 7 + 0) , line.charAt(5 + i * 7 + 1) , line.charAt(5 + i * 7 + 2) , line.charAt(5 + i * 7 + 3) , line.charAt(5 + i * 7 + 4) , line.charAt(5 + i * 7 + 5)};
    char colorHEX[6];
    for (int j=0;j<6;j++){
      colorHEX[j] = line.charAt(5 + i * 7 + j);
    }
    
    uint32_t color = StrToHex(colorHEX);
    Serial.printf("color in int: %d",color);
    strip.setPixelColor(i, color);
  }
}

int StrToHex(char str[])
{
  return (int) strtol(str, 0, 16);
}

