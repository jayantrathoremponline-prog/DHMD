
//  Commands: C = Calibrate, P = Pause, R = Resume

#include <SoftwareSerial.h>

// ---------- Bluetooth Setup ----------
SoftwareSerial BT(2, 3);   // RX=2, TX=3

// ---------- HC‑SR04 Pins ----------
const int trigPin = 9;
const int echoPin = 10;

// ---------- Global Variables ----------
float groundHeight = 0.0;
float lastHeight = 0.0;
bool paused = false;

// ---------- Simple Linear Correction ----------
const float CORRECTION_FACTOR = 1.0115;

float correctDistance(float raw) {
  return raw * CORRECTION_FACTOR;
}

// ---------- Read Distance ----------
float readDistance() {
  long totalDuration = 0;
  int validSamples = 0;
  for (int i = 0; i < 5; i++) {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    long duration = pulseIn(echoPin, HIGH, 30000);
    if (duration > 0) {
      totalDuration += duration;
      validSamples++;
    }
    delay(10);
  }
  if (validSamples == 0) return 0.0;
  float avgDuration = (float)totalDuration / validSamples;
  return (avgDuration * 0.01715);
}

// ---------- Calibration (debug messages only to USB) ----------
void calibrateGround() {
  Serial.println("Calibrating...");               // USB only
  float sum = 0;
  int valid = 0;
  for (int i = 0; i < 10; i++) {
    float raw = readDistance();
    if (raw > 0) {
      sum += correctDistance(raw);
      valid++;
    }
    delay(50);
  }
  if (valid > 0) {
    groundHeight = sum / valid;
    Serial.print("Ground: ");
    Serial.println(groundHeight, 2);              // USB only
    // (no BT output here – app only gets numbers)
  } else {
    Serial.println("Calibration failed!");        // USB only
  }
}

// ---------- Process Commands ----------
void processCommand(char cmd) {
  char c = toupper(cmd);
  if (c == 'C') {
    calibrateGround();
  } else if (c == 'P') {
    paused = true;
    BT.println("Paused");       
    Serial.println("Paused");
  } else if (c == 'R') {
    paused = false;
    BT.println("Resumed");
    Serial.println("Resumed");
  }
}

// ---------- Setup ----------
void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  Serial.begin(115200);          // USB debug (high baud)
  BT.begin(9600);               // HC‑05 baud

  while (!Serial) { ; }
  delay(1000);

  Serial.println("--- System Initializing ---");
  Serial.println("Commands: C=Calibrate, P=Pause, R=Resume");

  calibrateGround();
  BT.println("Ready.");          // first message to app (optional)
  Serial.println("Ready.");
}

// ---------- Main Loop ----------
void loop() {
  // Check BOTH USB Serial AND Bluetooth for commands
  if (Serial.available()) {
    char cmd = Serial.read();
    processCommand(cmd);
  }
  if (BT.available()) {
    char cmd = BT.read();
    processCommand(cmd);
  }

  // Read height if not paused
  if (!paused) {
    float raw = readDistance();
    if (raw > 0) {
      float currentDistance = correctDistance(raw);
      if (currentDistance > 0 && currentDistance < groundHeight) {
        lastHeight = groundHeight - currentDistance;
      }
    }
  }

  // Send ONLY numeric data over Bluetooth (two numbers, space-separated)
   BT.print("");
  BT.print(groundHeight, 2);
  BT.print(" ");
  BT.println(lastHeight, 2);

  // Also send to USB for monitoring (same format)
  Serial.print(groundHeight, 2);
  Serial.print(" ");
  Serial.println(lastHeight, 2);

  delay(500);
}
