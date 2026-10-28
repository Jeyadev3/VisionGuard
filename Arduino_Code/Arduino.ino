#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// ==========================================
// PINS
// ==========================================

#define TRIG1 2
#define ECHO1 3

#define TRIG2 4
#define ECHO2 5

#define BUZZER 6
#define SOS 7

#define TEMP_PIN 8

#define LED 13

// ==========================================
// TEMPERATURE SETTINGS
// ==========================================

const float FIRE_ON_TEMP = 50.0;
const float FIRE_OFF_TEMP = 48.0;

// ==========================================
// DS18B20
// ==========================================

OneWire oneWire(TEMP_PIN);
DallasTemperature sensors(&oneWire);

bool fireActive = false;

// ==========================================
// FIRE ALARM TIMING
// ==========================================

unsigned long lastFireBeep = 0;
bool fireBeepState = false;

// ==========================================
// SERIAL TIMING
// ==========================================

unsigned long lastSerial = 0;

// ==========================================
// SYSTEM MODE
// ==========================================

enum SystemMode {
  MODE_SAFE,
  MODE_OBJECT,
  MODE_OBSTACLE,
  MODE_DANGER,
  MODE_STAIR,
  MODE_FIRE,
  MODE_SOS
};

SystemMode currentMode = MODE_SAFE;
SystemMode previousMode = MODE_SAFE;

// ==========================================
// FRONT DISTANCE
// ==========================================

long getFrontDistance() {

  digitalWrite(TRIG1, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG1, HIGH);
  delayMicroseconds(10);

  digitalWrite(TRIG1, LOW);

  long duration = pulseIn(ECHO1, HIGH, 30000);

  if (duration == 0)
    return 400;

  return duration / 58;
}

// ==========================================
// DOWN DISTANCE
// ==========================================

long getDownDistance() {

  digitalWrite(TRIG2, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG2, HIGH);
  delayMicroseconds(10);

  digitalWrite(TRIG2, LOW);

  long duration = pulseIn(ECHO2, HIGH, 30000);

  if (duration == 0)
    return 400;

  return duration / 58;
}

// ==========================================
// LCD
// ==========================================

void showScreen(String line1, String line2) {

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print(line1);

  lcd.setCursor(0, 1);
  lcd.print(line2);
}

// ==========================================
// FIRE ALARM
// ==========================================

void updateFireAlarm() {

  unsigned long now = millis();

  if (now - lastFireBeep >= 180) {

    lastFireBeep = now;

    fireBeepState = !fireBeepState;

    if (fireBeepState) {

      tone(BUZZER, 2000);
      digitalWrite(LED, HIGH);

    } else {

      noTone(BUZZER);
      digitalWrite(LED, LOW);
    }
  }
}

// ==========================================
// NORMAL BEEP
// ==========================================

void beep(int frequency, int onTime, int offTime) {

  tone(BUZZER, frequency);
  digitalWrite(LED, HIGH);

  delay(onTime);

  noTone(BUZZER);
  digitalWrite(LED, LOW);

  delay(offTime);
}

// ==========================================
// SETUP
// ==========================================

void setup() {

  pinMode(TRIG1, OUTPUT);
  pinMode(ECHO1, INPUT);

  pinMode(TRIG2, OUTPUT);
  pinMode(ECHO2, INPUT);

  pinMode(BUZZER, OUTPUT);
  pinMode(LED, OUTPUT);

  pinMode(SOS, INPUT_PULLUP);

  Serial.begin(9600);

  // Start temperature sensor
  sensors.begin();

  // Faster DS18B20 conversion
  sensors.setResolution(9);

  lcd.init();
  lcd.backlight();

  showScreen("VisionGuard", "Starting...");

  beep(1000, 150, 150);
  beep(1000, 150, 150);

  delay(500);
}

// ==========================================
// MAIN LOOP
// ==========================================

void loop() {

  // ========================================
  // READ TEMPERATURE
  // ========================================

  sensors.requestTemperatures();

  float temperature = sensors.getTempCByIndex(0);

  // ========================================
  // READ ULTRASONIC
  // ========================================

  long front = getFrontDistance();
  long down = getDownDistance();

  // ========================================
  // FIRE STATE
  // ========================================

  if (!fireActive && temperature >= FIRE_ON_TEMP) {

    fireActive = true;
  }

  if (fireActive && temperature <= FIRE_OFF_TEMP) {

    fireActive = false;

    noTone(BUZZER);
    digitalWrite(LED, LOW);

    fireBeepState = false;
  }

  // ========================================
  // SERIAL MONITOR
  // ========================================

  if (millis() - lastSerial >= 200) {

    lastSerial = millis();

    Serial.print("Front : ");
    Serial.print(front);

    Serial.print("   Down : ");
    Serial.print(down);

    Serial.print("   Temperature : ");
    Serial.print(temperature);
    Serial.println(" C");
  }

  // ========================================
  // DETERMINE PRIORITY
  // ========================================

  if (digitalRead(SOS) == LOW) {

    currentMode = MODE_SOS;
  }

  else if (fireActive) {

    currentMode = MODE_FIRE;
  }

  else if (front <= 15) {

    currentMode = MODE_DANGER;
  }

  else if (front <= 30) {

    currentMode = MODE_OBSTACLE;
  }

  else if (front <= 60) {

    currentMode = MODE_OBJECT;
  }

  else if (down > 10) {

    currentMode = MODE_STAIR;
  }

  else {

    currentMode = MODE_SAFE;
  }

  // ========================================
  // HANDLE MODE CHANGE
  // ========================================

  if (currentMode != previousMode) {

    if (currentMode == MODE_FIRE) {

      showScreen("FIRE DETECTED", "MOVE AWAY");

      Serial.println("==================================");
      Serial.println(" FIRE HAZARD DETECTED ");
      Serial.print(" Temperature : ");
      Serial.print(temperature);
      Serial.println(" C");
      Serial.println(" ACTION : MOVE AWAY");
      Serial.println("==================================");

      fireBeepState = false;
      lastFireBeep = millis();
    }

    else if (currentMode == MODE_SOS) {

      showScreen("SOS ACTIVATED", "Sending Alert...");

      Serial.println("==================================");
      Serial.println(" EMERGENCY ALERT INITIATED ");
      Serial.println(" Alert Status : SENT (SIMULATED)");
      Serial.println(" Location : GPS Module Not Connected");
      Serial.println("==================================");
    }

    else if (currentMode == MODE_DANGER) {

      showScreen("DANGER!", "Very Close");
    }

    else if (currentMode == MODE_OBSTACLE) {

      showScreen("Obstacle", String(front) + " cm");
    }

    else if (currentMode == MODE_OBJECT) {

      showScreen("Object Ahead", String(front) + " cm");
    }

    else if (currentMode == MODE_STAIR) {

      showScreen("STAIR AHEAD", "STOP");
    }

    else {

      showScreen("PATH CLEAR", "Monitoring");
    }

    previousMode = currentMode;
  }

  // ========================================
  // FIRE MODE
  // ========================================

  if (currentMode == MODE_FIRE) {

    updateFireAlarm();

    // Immediately return to beginning
    // so temperature is checked again.
    return;
  }

  // ========================================
  // SOS MODE
  // ========================================

  if (currentMode == MODE_SOS) {

    tone(BUZZER, 1200);
    digitalWrite(LED, HIGH);

    delay(200);

    noTone(BUZZER);
    digitalWrite(LED, LOW);

    delay(200);

    return;
  }

  // ========================================
  // NORMAL WARNINGS
  // ========================================

  if (currentMode == MODE_DANGER) {

    beep(1500, 100, 50);
  }

  else if (currentMode == MODE_OBSTACLE) {

    beep(1200, 200, 100);
  }

  else if (currentMode == MODE_OBJECT) {

    beep(800, 300, 200);
  }

  else if (currentMode == MODE_STAIR) {

    beep(1800, 150, 80);
  }

  else {

    noTone(BUZZER);
    digitalWrite(LED, LOW);
  }

  delay(20);
}