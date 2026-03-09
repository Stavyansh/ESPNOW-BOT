# ESP32 Wireless Car Control with Fire Detection

A small robotics project implementing a wireless rover using two ESP32 boards communicating through ESP-NOW. The rover can be controlled using WASD commands and includes a flame sensor with a buzzer for fire detection and alerting.

---

## Overview
- Wireless robotic car controlled using ESP32 and ESP-NOW communication.
- Movement commands sent from a transmitter ESP32 and executed by a receiver ESP32.
- Rover supports forward, backward, left, right, and stop commands.
- Flame sensor detects nearby fire and activates a buzzer for alert.

---

## System Architecture
- Two ESP32 boards used in a transmitter receiver configuration.
- Transmitter sends character commands (`w`, `a`, `s`, `d`, `space`) through ESP-NOW.
- Receiver interprets commands and drives motors using an L298N motor driver.
- Flame sensor continuously monitors the environment for fire.
- Buzzer triggers when flame intensity crosses the threshold.

---

## Hardware Components
- 2 × ESP32 development boards
- 1 × L298N motor driver
- 2 × DC motors with rover chassis
- 1 × flame sensor module
- 1 × active buzzer
- battery pack (12V)
- jumper wires and breadboard

---

## Software
- developed using Arduino IDE
- programmed in Arduino C/C++
- main libraries used:
  - `WiFi.h`
  - `esp_now.h`

---

## Implementation
- phase 1: built a wired prototype using Arduino Uno and L298N to test motor control
- phase 2: implemented ESP-NOW wireless communication between two ESP32 boards
- phase 3: integrated flame sensor and buzzer for fire detection alerts

---

## Controls

| Key | Action |
|----|----|
| W | move forward |
| S | move backward |
| A | turn left |
| D | turn right |
| Space | stop |

---

## Performance
- wireless range approximately 50 meters in open space
- response time under 100 ms
- flame detection triggers buzzer within about 1 second
- stable rover movement after calibration of motor speed and power supply

---

## Challenges
- one ESP32 and two L298N modules were damaged due to power and polarity issues during development
- loose jumper connections occasionally caused signal interruptions
- flame sensor range limited to roughly 20–30 cm
- ESP-NOW communication used in one direction only

---

## Future Improvements
- add bidirectional communication or LoRa for telemetry feedback
- integrate ultrasonic sensors for obstacle avoidance
- add ESP32-CAM for visual fire detection
- replace L298N with TB6612FNG motor driver for better efficiency
- develop a mobile application for control

---

## Team
- Stavyansh Mishra
- Aryan Singh
- Saatwik Gupta

Robotics and Automation course project  
Vellore Institute of Technology
