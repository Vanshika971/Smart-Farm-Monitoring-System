Wemos D1 mini code that we need to show in presentation Final Draft!!!!!!!!!!!

#include <SPI.h>
#include <LoRa.h>
#include <DHT.h>

#define DHTPIN D3   
#define DHTTYPE DHT11
#define MOISTURE_PIN A0  

#define LORA_SS    D8       // NSS
#define LORA_RST   D0       // RST
#define LORA_DIO0  D1       // DIO0

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();

  LoRa.setPins(LORA_SS, LORA_RST, LORA_DIO0);
  if (!LoRa.begin(433E6)) {
    Serial.println("LoRa init failed. Check wiring.");
    while (1);
  }

  Serial.println("LoRa Sender ready");
}

void loop() {
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();
  int moisture = analogRead(MOISTURE_PIN);

  if (isnan(temp) || isnan(hum)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  String payload = "Temperature:" + String(temp, 1) + ",Humidity:" + String(hum, 1) + ",Moisture:" + String(moisture);


  LoRa.beginPacket();
  LoRa.print(payload);
  LoRa.endPacket();

  Serial.println("Sent: " + payload);
  delay(2000);  
}