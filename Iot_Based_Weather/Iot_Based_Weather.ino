#define BLYNK_TEMPLATE_ID "TMPL30cqT1jNw"
#define BLYNK_TEMPLATE_NAME "WEATHER MONITORING STATION"
#define BLYNK_AUTH_TOKEN "mhe3XHyIN_b2ma76iTc5YIeemf8PKODM"

#include <Wire.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <ThingSpeak.h>
#include <Adafruit_BMP085.h>
#include <DHT.h>

char ssid[] = "Pavankumar";
char pass[] = "33023302";

#define DHTPIN D4
#define DHTTYPE DHT11

unsigned long channelID = 2258867;
const char *apiKey= "8VA77EPSE8O7GUUL";
WiFiClient client;

Adafruit_BMP085 bmp;
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  bmp.begin();
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  WiFi.begin(ssid,pass);
  dht.begin();
  ThingSpeak.begin(client);
}

void loop() {
  float pressure = bmp.readPressure() / 100.0F;
  float altitude = bmp.readAltitude();

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  
  Blynk.virtualWrite(V0,pressure);
  Blynk.virtualWrite(V1,altitude);
  Blynk.virtualWrite(V2,temperature);
  Blynk.virtualWrite(V3,humidity);

  ThingSpeak.setField(1,pressure);
  ThingSpeak.setField(2,altitude);
  ThingSpeak.setField(3,temperature);
  ThingSpeak.setField(4,humidity);

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println("%");

  Serial.print("Pressure: ");
  Serial.print(pressure);
  Serial.println(" hPa");

  Serial.print("Altitude: ");
  Serial.print(altitude);
  Serial.println(" m");

  int response = ThingSpeak.writeFields(channelID, apiKey);
  if (response == 200) {
    Serial.println("Data sent to ThingSpeak successfully.");
  } else {
    Serial.println("Error sending data to ThingSpeak. HTTP error code: " + String(response));
  }

  delay(16000);
}
