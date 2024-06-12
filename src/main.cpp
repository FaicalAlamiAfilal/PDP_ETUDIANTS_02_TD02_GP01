#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <WiFi.h>
#include <WiFiClient.h>

// Define the pins that we will use
#define CAPTEUR 33
#define LED 26
#define DHTTYPE DHT11

DHT_Unified dht(CAPTEUR, DHTTYPE);

#define BLYNK_TEMPLATE_ID "TMPL5JXuJgV9U"
#define BLYNK_TEMPLATE_NAME "PDP Modèle"
#define BLYNK_AUTH_TOKEN "-tK0ORbUoKQX9ou3eRdH1SKoBqTHRhIk"

#include <BlynkSimpleEsp32.h>

#define BLYNK_PRINT Serial

char ssid[] = "Livebox-C1C0";
char pass[] = "JQgwZA7jfz3TmTNZNN";
BLYNK_WRITE(V2)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V0 to a variable
  Serial.print("Received value from Blynk: ");
  Serial.println(pinValue);
  digitalWrite(LED,!pinValue);
  // Delay is only there so that we get a chance to see the LED value properly.
  delay(1000);
}

void setup() {
  
  // Setup pins
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  

  // Begin serial communication
  Serial.begin(9600);
  delay(100);

  // begin the Blynk session
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  Blynk.run();
  Blynk.syncVirtual(V2);

  // Start listening to the DHT11
  dht.begin();

  sensors_event_t event;

  // Get temperature event and print its value
  float temp_measure = -999.0;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
  } else {
    Serial.print(F("Temperature: "));
    Serial.print(event.temperature);
    Serial.println(F("°C"));
    temp_measure = event.temperature;
  }

  // Get humidity event and print its value.
  float relative_humidity_measure = -999.0;
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));
  } else {
    Serial.print(F("Humidity: "));
    Serial.print(event.relative_humidity);
    Serial.println(F("%"));
    relative_humidity_measure = event.relative_humidity;
  }

  // Send data to Blynk
  Blynk.virtualWrite(V1, temp_measure);
  Blynk.virtualWrite(V0, relative_humidity_measure); 
  // ...

  Serial.println("Going to sleep for 5 seconds...");
  delay(100);
  ESP.deepSleep(5e6);
}

void loop() {
  // Not needed anymore, the function is kept so PlatformIO does not complain.
}
