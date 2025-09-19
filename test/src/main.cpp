#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>

#define POT_PIN 36  // Potentiometer connected to GPIO36

// MAC address of the ESP32-C3 Receiver
uint8_t receiverAddress[] = {0x98, 0x3D, 0xAE, 0x52, 0xD2, 0xB8};

// Structure to send data
typedef struct struct_message {
  int angle;
} struct_message;

struct_message outgoingData;

// Callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  // Optional debug
  // Serial.print("Send Status: ");
  // Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
}

void setup() {
  Serial.begin(9600);

  // Initialize Wi-Fi in STA mode
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register send callback
  esp_now_register_send_cb(OnDataSent);

    // Add peer (receiver)
  esp_now_peer_info_t peerInfo;
  memset(&peerInfo, 0, sizeof(peerInfo));   // <-- clear struct first
  memcpy(peerInfo.peer_addr, receiverAddress, 6);
  peerInfo.channel = 0;      // use current channel
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

}

void loop() {
  // Read potentiometer
  int potValue = analogRead(POT_PIN);

  // Map 0–4095 to 0–180
  outgoingData.angle = map(potValue, 0, 4095, 0, 180);

  // Send angle via ESP-NOW
  esp_now_send(receiverAddress, (uint8_t *)&outgoingData, sizeof(outgoingData));

  // Small delay to avoid spamming
  delay(100); 
}
