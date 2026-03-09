#include <esp_now.h>
#include <WiFi.h> 

#define IN1 13   // Left motor input 1
#define IN2 12   // Left motor input 2
#define IN3 14   // Right motor input 1
#define IN4 27   // Right motor input 2
#define FLAME_SENSOR 34   // IR flame sensor output pin
#define BUZZER_PIN   26   // Active buzzer pin
char command = ' ';
bool flameDetected = false;
void stopCar() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
void moveCar(char cmd) {
  cmd = tolower(cmd);
  switch (cmd) {
    case 'w': // Forward
      digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
      digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
      break;
    case 's': // Backward
      digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
      digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
      break;
    case 'a': // Left
      digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
      digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
      break;
    case 'd': // Right
      digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
      break;
    default: // Stop
      stopCar();
      break;
  }
}
void onDataRecv(const esp_now_recv_info * info, const uint8_t *incomingData, int len) {
  command = (char)*incomingData;
  Serial.print("Received: ");
  Serial.println(command);
  moveCar(command);
}
void setup() {
  Serial.begin(115200);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  stopCar();
  pinMode(FLAME_SENSOR, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed!");
    return;
  }
  esp_now_register_recv_cb(onDataRecv);
  Serial.println("Receiver ready on new pins + flame sensor.");
}
void loop() {
  int flameValue = digitalRead(FLAME_SENSOR);
  if (flameValue == LOW) {
    if (!flameDetected) {
      flameDetected = true;
      Serial.println(" Flame detected! Buzzer ON");
    }
    digitalWrite(BUZZER_PIN, HIGH);
  } else {
    if (flameDetected) {
      flameDetected = false;
      Serial.println(" No flame. Buzzer OFF");
    }
    digitalWrite(BUZZER_PIN, LOW);
  }
  delay(100);
}
