# VisionGuard: Smart Mobility Assistance System for the Visually Impaired

### An Arduino-Based Assistive Mobility Prototype

## 📌 Project Overview

VisionGuard is a smart mobility assistance prototype designed to help demonstrate how electronic sensors and alerts can support visually impaired individuals in identifying nearby obstacles and potential environmental hazards.

The system uses an Arduino Uno, ultrasonic sensors, a temperature sensor, a buzzer, an LED, an LCD, and an SOS push button. The project is developed and tested through the Wokwi simulation environment.

## 🎯 Objectives

* Detect obstacles in front of the user using an ultrasonic sensor.
* Identify possible stairs or drop-offs using a downward-facing ultrasonic sensor.
* Provide temperature-based hazard warnings.
* Display system status on an LCD.
* Provide audible and visual alerts.
* Demonstrate an SOS alert using a push button.

## ✨ Features

### 1. Obstacle Detection

The front-facing HC-SR04 ultrasonic sensor measures the distance to objects and classifies the detected condition according to configured distance thresholds.

### 2. Stair and Drop-Off Indication

The downward-facing HC-SR04 sensor checks distance changes that may indicate a possible stair or drop-off.

### 3. Temperature-Based Hazard Warning

The DS18B20 temperature sensor measures its own temperature. A warning is activated when the configured temperature threshold is reached.

### 4. Audible and Visual Alerts

A buzzer and LED provide alerts when configured conditions are detected.

### 5. LCD Status Display

A 16×2 I2C LCD displays system messages and the current alert status.

### 6. SOS Alert

A push button activates a simulated SOS alert mode. The current prototype does not send actual SMS messages or GPS coordinates.

## 🧰 Hardware Components

| Component                     | Purpose                                        |
| ----------------------------- | ---------------------------------------------- |
| Arduino Uno                   | Main microcontroller                           |
| HC-SR04 Ultrasonic Sensor × 2 | Front obstacle and downward distance detection |
| DS18B20 Temperature Sensor    | Temperature monitoring                         |
| Buzzer                        | Audible alerts                                 |
| LED                           | Visual alerts                                  |
| Push Button                   | SOS activation                                 |
| 16×2 I2C LCD                  | Status display                                 |

## 🔌 Pin Configuration

| Component                | Arduino Pin |
| ------------------------ | ----------- |
| Front ultrasonic TRIG    | D2          |
| Front ultrasonic ECHO    | D3          |
| Downward ultrasonic TRIG | D4          |
| Downward ultrasonic ECHO | D5          |
| Buzzer                   | D6          |
| SOS push button          | D7          |
| DS18B20 data             | D8          |
| LED                      | D13         |
| LCD SDA                  | A4          |
| LCD SCL                  | A5          |

## 💻 Software and Tools

* Arduino C/C++
* Wokwi Arduino Simulator
* OneWire library
* DallasTemperature library
* LiquidCrystal_I2C library

## ⚙️ Working Principle

1. The Arduino initializes the sensors, LCD, buzzer, LED, and SOS button.
2. The front ultrasonic sensor measures the distance to nearby objects.
3. The downward ultrasonic sensor checks for distance conditions that may indicate a stair or drop-off.
4. The DS18B20 measures temperature and activates a warning when the configured threshold is reached.
5. The SOS button can activate the simulated emergency alert.
6. The Arduino selects the applicable alert, updates the LCD, and controls the buzzer and LED.
7. The monitoring process repeats continuously.

## 🧪 Simulation

The project is implemented in Wokwi. The repository contains the Arduino source code and circuit configuration used for the simulation.

To explore the project:

1. Open the `Arduino_Code` folder to view the Arduino program.
2. Open the `Wokwi_Simulation` folder to view the circuit configuration.
3. Refer to the `Documentation` folder for the case study and presentation.

## 📁 Repository Structure

```text
VisionGuard/
├── README.md
├── Arduino_Code/
│   └── VisionGuard.ino
├── Wokwi_Simulation/
│   ├── diagram.json
│   └── libraries.txt
└── Documentation/
    ├── VisionGuard_Case_Study_Submission_Ready.docx
    └── VisionGuard_Presentation.pptx
```

*The filenames shown above are examples of the intended organization. Actual filenames may differ.*

## ⚠️ Limitations

* The DS18B20 measures its own temperature; it is not a long-range flame detector.
* The SOS feature is simulated and does not send real emergency messages or GPS coordinates.
* Ultrasonic sensor readings may vary depending on object shape, angle, and environmental conditions.
* The project is a prototype and has not been validated as a real-world mobility or safety device.

## 🔮 Future Enhancements

* Integrate GPS and a communication module for emergency location sharing.
* Investigate suitable non-contact thermal or flame-detection hardware.
* Add vibration-based alerts for improved accessibility.
* Improve sensor reliability and alert handling.
* Conduct practical testing and user-centered evaluation.

---

**VisionGuard — Demonstrating sensor-based assistance for safer mobility.**
