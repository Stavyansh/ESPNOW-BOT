
const int DIR1 = 9;  // Direction pin for Motor 1
const int PWM1 = 5;  // PWM pin for Motor 1 (connected to an LEDC channel)
const int DIR2 = 10; // Direction pin for Motor 2
const int PWM2 = 6;  // PWM pin for Motor 2 (connected to an LEDC channel)

#define LEDC_CHANNEL_0 0
#define LEDC_CHANNEL_1 1

#define LEDC_TIMER_BIT 8      // Resolution of PWM duty cycle (8 bits = 0-255)
#define LEDC_BASE_FREQ 5000   // Base frequency for PWM (Hz)

// Variables to store speed and turn values
int speed = 0; // Overall speed of the robot (-224 to 224)
int turn = 0;  // Turning bias (-64 to 64)

void setup() {

  pinMode(DIR1, OUTPUT);
  pinMode(DIR2, OUTPUT);

  Serial.begin(9600);
  Serial.println("ESP32 Motor Control Ready!");
  Serial.println("Use W/A/S/D to move, Space to stop.");


    ledcSetup(LEDC_CHANNEL_0, LEDC_BASE_FREQ, LEDC_TIMER_BIT);
  ledcSetup(LEDC_CHANNEL_1, LEDC_BASE_FREQ, LEDC_TIMER_BIT);


  ledcAttachPin(PWM1, LEDC_CHANNEL_0);
  ledcAttachPin(PWM2, LEDC_CHANNEL_1);
}

void loop() {
  
  if (Serial.available()) {
    char c = Serial.read(); // Read the character

    // Process the input character
    switch (c) {
      case 'w': // Increase speed (forward)
        speed += 32;
        break;
      case 's': // Decrease speed (backward)
        speed -= 32;
        break;
      case 'a': // Turn left (increase left motor speed, decrease right)
        turn -= 16;
        break;
      case 'd': // Turn right (decrease left motor speed, increase right)
        turn += 16;
        break;
      case ' ': // Stop motors
        speed = 0;
        turn = 0;
        break;
    }

  
    speed = constrain(speed, -224, 224); // Max speed for motors
    turn = constrain(turn, -64, 64);   // Max turn bias

  
    int l = speed + turn;
    int r = speed - turn;

    setMotor(DIR1, LEDC_CHANNEL_0, l); // Motor 1 (Left)
    setMotor(DIR2, LEDC_CHANNEL_1, r); // Motor 2 (Right)
  }
}

void setMotor(int dirPin, int ledcChannel, int val) {

  digitalWrite(dirPin, val >= 0);

 
  ledcWrite(ledcChannel, min(abs(val), 255));
}
