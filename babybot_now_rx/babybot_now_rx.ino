#include <WiFi.h>
#include <esp_now.h>

// Motor pins
#define ENA 13
#define IN1 12
#define IN2 14
#define ENB 15
#define IN3 27
#define IN4 26
#define MOTOR_SPEED 200

void forward() {
  digitalWrite(IN1,HIGH); digitalWrite(IN2,LOW);
  digitalWrite(IN3,HIGH); digitalWrite(IN4,LOW);
  ledcWrite(ENA, MOTOR_SPEED);
  ledcWrite(ENB, MOTOR_SPEED);
}

void backward() {
  digitalWrite(IN1,LOW); digitalWrite(IN2,HIGH);
  digitalWrite(IN3,LOW); digitalWrite(IN4,HIGH);
  ledcWrite(ENA, MOTOR_SPEED);
  ledcWrite(ENB, MOTOR_SPEED);
}

void left() {
  digitalWrite(IN1,LOW); digitalWrite(IN2,HIGH);
  digitalWrite(IN3,HIGH); digitalWrite(IN4,LOW);
  ledcWrite(ENA, MOTOR_SPEED);
  ledcWrite(ENB, MOTOR_SPEED);
}

void right() {
  digitalWrite(IN1,HIGH); digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW); digitalWrite(IN4,HIGH);
  ledcWrite(ENA, MOTOR_SPEED);
  ledcWrite(ENB, MOTOR_SPEED);
}

void stopMotors() {
  digitalWrite(IN1,LOW); digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW); digitalWrite(IN4,LOW);
  ledcWrite(ENA,0);
  ledcWrite(ENB,0);
}

// Correct callback signature
void OnDataRecv(const esp_now_recv_info_t *info, const uint8_t *incomingData, int len) {
  if(len < 1) return;
  char cmd = incomingData[0];
  Serial.print("Received: "); Serial.println(cmd);

  switch(cmd){
    case 'w': forward(); break;
    case 's': backward(); break;
    case 'a': left(); break;
    case 'd': right(); break;
    default: stopMotors(); break;
  }
}

void setup() {
  Serial.begin(115200);

  // Motor pins
  pinMode(IN1,OUTPUT); pinMode(IN2,OUTPUT);
  pinMode(IN3,OUTPUT); pinMode(IN4,OUTPUT);

  // PWM pins (ESP32 v3.x+)
  ledcAttach(ENA,1000,8);
  ledcAttach(ENB,1000,8);

  // Wi-Fi must be STA mode
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();  // disconnect from any network

  // Initialize ESP-NOW
  if(esp_now_init() != ESP_OK){
    Serial.println("ESP-NOW Init Failed");
    return;
  }

  // Register receive callback
  esp_now_register_recv_cb(OnDataRecv);

  Serial.println("Receiver ready. Waiting for commands...");
}

void loop(){}
