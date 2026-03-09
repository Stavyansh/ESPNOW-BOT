// Define the GPIO pins for motor control
// These pins are suitable for ESP32.
// GPIO 9 and 10 are common I/O pins.
// GPIO 5 and 6 are also common I/O pins and can be used for PWM.
const int DIR1 = 9;  // Direction pin for Motor 1
const int PWM1 = 5;  // PWM pin for Motor 1 (connected to an LEDC channel)
const int DIR2 = 10; // Direction pin for Motor 2
const int PWM2 = 6;  // PWM pin for Motor 2 (connected to an LEDC channel)

// Define LEDC channels for PWM on ESP32
// ESP32 has 16 LEDC channels (0-15). We'll use two of them.
#define LEDC_CHANNEL_0 0
#define LEDC_CHANNEL_1 1

// Define PWM frequency and resolution
// 5000 Hz is a common frequency for motor control.
// 8-bit resolution means values from 0 to 255, matching Arduino's analogWrite.
#define LEDC_TIMER_BIT 8      // Resolution of PWM duty cycle (8 bits = 0-255)
#define LEDC_BASE_FREQ 5000   // Base frequency for PWM (Hz)

// Variables to store speed and turn values
int speed = 0; // Overall speed of the robot (-224 to 224)
int turn = 0;  // Turning bias (-64 to 64)

void setup() {
  // Configure direction pins as outputs
  pinMode(DIR1, OUTPUT);
  pinMode(DIR2, OUTPUT);

  // Initialize Serial communication for debugging and input
  Serial.begin(9600);
  Serial.println("ESP32 Motor Control Ready!");
  Serial.println("Use W/A/S/D to move, Space to stop.");

  // --- ESP32 LEDC (PWM) Configuration ---
  // Configure LEDC timer for PWM
  // This sets up the frequency and resolution for our PWM signals.
    ledcSetup(LEDC_CHANNEL_0, LEDC_BASE_FREQ, LEDC_TIMER_BIT);
  ledcSetup(LEDC_CHANNEL_1, LEDC_BASE_FREQ, LEDC_TIMER_BIT);

  // Attach LEDC channels to the respective PWM pins
  // This connects our configured PWM channels to the physical GPIO pins.
  ledcAttachPin(PWM1, LEDC_CHANNEL_0);
  ledcAttachPin(PWM2, LEDC_CHANNEL_1);
}

void loop() {
  // Check if there's incoming data from the Serial monitor
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

    // Constrain speed and turn values to stay within safe operating limits
    // This prevents values from exceeding what the motor driver can handle.
    speed = constrain(speed, -224, 224); // Max speed for motors
    turn = constrain(turn, -64, 64);   // Max turn bias

    // Calculate individual motor speeds based on speed and turn
    // 'l' is for the left motor, 'r' for the right motor.
    // Adding 'turn' to 'speed' for one motor and subtracting for the other
    // creates the differential drive effect.
    int l = speed + turn;
    int r = speed - turn;

    // Set the motor speeds and directions
    setMotor(DIR1, LEDC_CHANNEL_0, l); // Motor 1 (Left)
    setMotor(DIR2, LEDC_CHANNEL_1, r); // Motor 2 (Right)
  }
}

/**
 * @brief Sets the speed and direction of a motor.
 *
 * This function controls a single motor connected to a direction pin and a PWM pin.
 * It uses ESP32's LEDC peripheral for PWM control.
 *
 * @param dirPin The GPIO pin connected to the direction input of the motor driver.
 * @param ledcChannel The LEDC channel assigned to the pwmPin.
 * @param val The speed value (-255 to 255). Positive values for one direction,
 * negative for the other. 0 for stop.
 */
void setMotor(int dirPin, int ledcChannel, int val) {
  // Set motor direction
  // If 'val' is positive or zero, set direction to HIGH (e.g., forward).
  // If 'val' is negative, set direction to LOW (e.g., backward).
  // The actual HIGH/LOW mapping depends on your motor driver and wiring.
  digitalWrite(dirPin, val >= 0);

  // Set motor speed using LEDC
  // abs(val) gets the magnitude of the speed.
  // min(abs(val), 255) ensures the value doesn't exceed 255 (8-bit resolution).
  // ledcWrite(channel, duty_cycle) sets the PWM duty cycle for the specified channel.
  ledcWrite(ledcChannel, min(abs(val), 255));
}

// Note: The 'constrain' function is part of the Arduino core and works on ESP32.
// If you were porting to a bare-metal ESP-IDF project, you would need to
// implement it manually (e.g., using if-statements or std::min/max).