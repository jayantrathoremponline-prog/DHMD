# Arduino Height Sensor with Bluetooth (HC‑05)
# DHMD

This folder contains the Arduino firmware for a **real‑time height measurement system** using an HC‑SR04 ultrasonic sensor and an HC‑05 Bluetooth module.  
The Arduino sends distance data to an MIT App Inventor Android app(DHMD) via Bluetooth, and responds to remote commands (`Calibrate`, `Pause`, `Resume`).

---

## 📦 Hardware Requirements

| Component                      | Quantity |
| Arduino Uno / Nano / Mega          | 1 |
| HC‑SR04 Ultrasonic Sensor          | 1 |
| HC‑05 Bluetooth Module             | 1 |
| Jumper Wires                       | 6–8 |
| 5V Power Supply (USB or external)  | 1 |

---

## 🔌 Wiring Diagram

Connect the components as follows:

| HC‑SR04 Pin | Arduino Pin |
| :---        | :--- |
| VCC         | 5V | (OR connect through external vcc of  constant 5v power supply)
| GND         | GND | (OR connect through external ground of constant 5v power supply)
| TRIG        | **9** |
| ECHO        | **10** |

| HC‑05 Pin | Arduino Pin |
| :--- | :--- |
| VCC | 5V | (OR connect through external vcc of  constant 5v power supply)
| GND | GND | (OR connect through external ground of constant 5v power supply)
| TX | **2** (SoftwareSerial RX) |
| RX | **3** (SoftwareSerial TX) |

> ⚠️ **Important:** The HC‑05 is powered by 5V, but its RX pin is **3.3V tolerant**. If you have a 5V Arduino, it is generally safe, but for long‑term reliability, use a voltage divider (2× 1kΩ resistors) on the Arduino TX → HC‑05 RX line.

---

## 📂 Arduino Code File

**File name:** `DHMD.ino`

### How to Upload

1. Open `DHMD.ino` in the Arduino IDE.
2. Select your board (Tools → Board → Arduino Uno/Nano/etc.).
3. Select the correct port (Tools → Port).
4. Click **Upload**.

> ⚠️ The HC‑05 uses **pins 2 and 3** (SoftwareSerial), so the USB Serial Monitor (pins 0,1) remains free for debugging.

---

## ⚙️ Configuration (Constants You Can Tweak)

```cpp
const float CORRECTION_FACTOR = 1.0115;   // Multiplier to fix sensor drift
const int FILTER_SIZE = 10;               // Moving average window (smoothing)
