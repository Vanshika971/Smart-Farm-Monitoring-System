//NodeMCU Uploaded Code Receiver Code 

#define BLYNK_TEMPLATE_ID "TMPL308CIC-1N"
#define BLYNK_TEMPLATE_NAME "LoRaWAN NodeMCU Project"
#define BLYNK_AUTH_TOKEN "NhGzkmUuPv0UTZ9CtC-WC7ePQXgRduk8"

#include <SPI.h>
#include <LoRa.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// --- Wi-Fi credentials ---
char ssid[] = "leftover";
char pass[] = "leftover";

// --- Blynk Auth Token ---
char auth[] = "NhGzkmUuPv0UTZ9CtC-WC7ePQXgRduk8";

// LoRa pins for ESP8266 (NodeMCU)
#define LORA_SS    D8  // GPIO15
#define LORA_RST   D0  // GPIO16
#define LORA_DIO0  D1  // GPIO5

void setup() {
  Serial.begin(9600);

  // Connect to WiFi and Blynk
  WiFi.begin(ssid, pass);
  Blynk.config(auth);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");

  // Setup LoRa
  LoRa.setPins(LORA_SS, LORA_RST, LORA_DIO0);

  if (!LoRa.begin(433E6)) {
    Serial.println("LoRa init failed. Check connections.");
    while (1);
  }
  Serial.println("LoRa Receiver Ready");
}

void loop() {
  Blynk.run();

  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    String incoming = "";
    while (LoRa.available()) {
      incoming += (char)LoRa.read();
    }
    Serial.println("Received: " + incoming);

    // parse format:
    // temperature:12.3 , humidity:12.3 , Moisture:710

    float temperature = 0, humidity = 0;
    int moisture = 0;

    int tIndex = incoming.indexOf("Temperature:");
    int hIndex = incoming.indexOf("Humidity:");
    int mIndex = incoming.indexOf("Moisture:");

    if (tIndex != -1 && hIndex != -1 && mIndex != -1) {
      temperature = incoming.substring(tIndex + 11, incoming.indexOf(',', tIndex)).toFloat();
      humidity = incoming.substring(hIndex + 9, incoming.indexOf(',', hIndex)).toFloat();
      moisture = incoming.substring(mIndex + 9).toInt();

      Serial.print("Temp: "); Serial.println(temperature);
      Serial.print("Humidity: "); Serial.println(humidity);
      Serial.print("Soil Moisture: "); Serial.println(moisture);

      // Send to Blynk
      Blynk.virtualWrite(V0, temperature); // V0: Temperature
      Blynk.virtualWrite(V1, humidity);    // V1: Humidity
      Blynk.virtualWrite(V2, moisture);    // V2: Soil Moisture
    } else {
      Serial.println("Invalid data format");
    }
  }
}