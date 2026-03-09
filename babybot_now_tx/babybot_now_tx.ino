#include <esp_now.h>
#include <WiFi.h>
uint8_t receiverAddress[] = {0x08, 0xA6, 0xF7, 0xB1, 0x12, 0xF8};
typedef struct struct_message {
  char command;
} struct_message;
struct_message myData;
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("Send Status: ");
  if (status == ESP_NOW_SEND_SUCCESS) {
    Serial.println(" Success");
  } else {
    Serial.println(" Failed");
  }
}
void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n=================================");
  Serial.println("ESP32 Wireless Car Controller");
  Serial.println("=================================");
  WiFi.mode(WIFI_STA);
  Serial.print("Sender MAC: ");
  Serial.println(WiFi.macAddress());
  Serial.println();
  if (esp_now_init() != ESP_OK) {
    Serial.println(" ERROR: ESP-NOW Init Failed!");
    return;
  }
  Serial.println(" ESP-NOW Initialized");
  esp_now_register_send_cb(OnDataSent);
  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, receiverAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println(" ERROR: Failed to add peer!");
    return;
  }
  Serial.println(" Peer Added");
  Serial.println("\n=================================");
  Serial.println("Controller Ready!");
  Serial.println("=================================");
  Serial.println("Commands:");
  Serial.println("  W - Move Forward");
  Serial.println("  A - Turn Left");
  Serial.println("  S - Move Backward");
  Serial.println("  D - Turn Right");
  Serial.println("  X - Stop");
  Serial.println("=================================\n");
}
void loop() {
  if (Serial.available() > 0) {
    char input = Serial.read();
    if (input >= 'a' && input <= 'z') {
      input = input - 32;
    }
    if (input == 'W' || input == 'A' || input == 'S' || input == 'D' || input == 'X') {
      sendCommand(input);
    }
  }
}
void sendCommand(char cmd) {
  Serial.print("Sending: ");
  switch (cmd) {
    case 'W': Serial.println("FORWARD "); break;
    case 'A': Serial.println("LEFT "); break;
    case 'S': Serial.println("BACKWARD "); break;
    case 'D': Serial.println("RIGHT "); break;
    case 'X': Serial.println("STOP "); break;
  }
  myData.command = cmd;
  esp_err_t result = esp_now_send(receiverAddress, (uint8_t *)&myData, sizeof(myData));
  if (result != ESP_OK) {
    Serial.println(" ERROR: Sending failed!");
  }
  delay(50);
}
