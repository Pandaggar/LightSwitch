#include <WiFi.h>
#include <esp_now.h>
#include <Servo.h>

Servo myServo = Servo();

const int servoPin = 2;

// Structure to receive data
typedef struct struct_message {
  int angle;
} struct_message;

struct_message incomingData;

// Callback when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingDataBytes, int len) {
  memcpy(&incomingData, incomingDataBytes, sizeof(incomingData));

  // Write received angle to servo
  myServo.write(servoPin,incomingData.angle);
}

void setup() {
  Serial.begin(9600);
  setCpuFrequencyMhz(80); // default is 160


  // Initialize Wi-Fi in STA mode
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register receive callback
  esp_now_register_recv_cb(OnDataRecv);
  
}

void loop() {
  // Nothing needed here, servo updates happen inside the callback
}
