# TCS3200 Color Sensor Project

This project implements a color detection system using the TCS3200 color sensor with Arduino. The system captures RGB color values and converts them to HSV color space for more intuitive color analysis.

## Table of Contents
- [Overview](#overview)
- [Hardware Requirements](#hardware-requirements)
- [Circuit Diagram](#circuit-diagram)
- [Software Features](#software-features)
- [How It Works](#how-it-works)
- [Setup and Installation](#setup-and-installation)
- [Usage](#usage)
- [Code Structure](#code-structure)
- [Calibration](#calibration)
- [Troubleshooting](#troubleshooting)
- [Example Output](#example-output)

## Overview

The TCS3200 color sensor is a programmable color light-to-frequency converter that combines configurable silicon photodiodes and a current-to-frequency converter on a single monolithic CMOS integrated circuit. This project reads color values from the sensor and converts them from RGB to HSV color space for better color analysis and classification.

## Hardware Requirements

- Arduino board (Uno, Nano, or compatible)
- TCS3200 color sensor module
- Jumper wires
- Breadboard (optional)
- USB cable for programming

## Circuit Diagram

### Pin Connections

| TCS3200 Pin | Arduino Pin | Description |
|-------------|-------------|-------------|
| VCC | 5V | Power supply |
| GND | GND | Ground |
| S0 | Pin 4 | Frequency scaling select |
| S1 | Pin 5 | Frequency scaling select |
| S2 | Pin 6 | Photodiode type select |
| S3 | Pin 7 | Photodiode type select |
| OUT | Pin 8 | Frequency output |
| OE | GND | Output enable (always enabled) |

### Wiring Diagram
```
Arduino Uno          TCS3200
    5V    --------->   VCC
    GND   --------->   GND
    Pin 4 --------->   S0
    Pin 5 --------->   S1
    Pin 6 --------->   S2
    Pin 7 --------->   S3
    Pin 8 --------->   OUT
    GND   --------->   OE
```

## Software Features

- **RGB Color Detection**: Reads red, green, and blue color intensities
- **HSV Conversion**: Converts RGB values to HSV color space
- **Pulse Width Normalization**: Normalizes sensor readings to 0-255 range
- **Serial Output**: Real-time color data display via Serial Monitor
- **Configurable Frequency Scaling**: Set to 20% for optimal performance

## How It Works

### TCS3200 Operation
The TCS3200 sensor contains:
- 16 photodiodes (4 red, 4 green, 4 blue, 4 clear filters)
- Current-to-frequency converter
- Digital outputs for filter selection

### Color Detection Process
1. **Filter Selection**: S2 and S3 pins select which color filter to read
2. **Frequency Output**: Sensor outputs a square wave with frequency proportional to light intensity
3. **Pulse Width Measurement**: Arduino measures pulse width (inversely proportional to frequency)
4. **Normalization**: Pulse widths are normalized to 0-255 RGB values
5. **HSV Conversion**: RGB values are converted to HSV for better color analysis

### Filter Selection Logic
| S2 | S3 | Filter Type |
|----|----|----|
| LOW | LOW | Red |
| HIGH | HIGH | Green |
| LOW | HIGH | Blue |
| HIGH | LOW | Clear (no filter) |

## Setup and Installation

1. **Hardware Setup**:
   - Connect the TCS3200 sensor to Arduino according to the pin diagram
   - Ensure all connections are secure

2. **Software Setup**:
   - Open Arduino IDE
   - Copy the code from `code.cpp` into a new sketch
   - Select your Arduino board and port
   - Upload the code

3. **Testing**:
   - Open Serial Monitor (9600 baud rate)
   - Place different colored objects near the sensor
   - Observe the RGB and HSV values

## Usage

1. **Power on** the Arduino with the sensor connected
2. **Open Serial Monitor** at 9600 baud rate
3. **Place objects** of different colors close to the sensor (2-10mm distance)
4. **Observe readings** in the Serial Monitor:
   - RGB values (0-255 range)
   - HSV values (H: 0-360°, S: 0-1, V: 0-1)

### Best Practices
- Keep the sensor 2-10mm away from the object
- Use consistent lighting conditions
- Calibrate the sensor for your specific environment
- Allow the sensor to stabilize between readings

## Code Structure

### Main Components

#### 1. Pin Definitions
```cpp
#define S0 4        // Frequency scaling
#define S1 5        // Frequency scaling  
#define S2 6        // Color filter select
#define S3 7        // Color filter select
#define sensorOut 8 // Sensor output
```

#### 2. HSV Structure
```cpp
struct HSV {
  float h; // Hue (0-360°)
  float s; // Saturation (0-1)
  float v; // Value/Brightness (0-1)
};
```

#### 3. Key Functions

- **`getRedPW()`**: Reads red color pulse width
- **`getGreenPW()`**: Reads green color pulse width
- **`getBluePW()`**: Reads blue color pulse width
- **`normalizePW(int pw)`**: Converts pulse width to 0-255 RGB value
- **`rgb2hsv(float r, float g, float b)`**: Converts RGB to HSV color space

### Program Flow
1. Initialize pins and serial communication
2. Set frequency scaling to 20%
3. In main loop:
   - Read RGB pulse widths
   - Normalize to 0-255 range
   - Convert to HSV
   - Output values to Serial Monitor
   - Wait 500ms before next reading

## Complete Code Explanation

### 1. Header and Pin Definitions
```cpp
// ---------- TCS3200 Color Sensor Pins ----------
#define S0 4
#define S1 5
#define S2 6
#define S3 7
#define sensorOut 8
```
- **S0, S1**: Control the output frequency scaling (2%, 20%, or 100%)
- **S2, S3**: Select which photodiode type to read (Red, Green, Blue, or Clear)
- **sensorOut**: The pin that receives the frequency output from the sensor

### 2. Global Variables and Constants
```cpp
// ---------- Variables ----------
int redPW, greenPW, bluePW;

// ---------- Max Pulse Width for normalization ----------
const int MAX_PW = 2000;
```
- **redPW, greenPW, bluePW**: Store the pulse width readings for each color
- **MAX_PW**: Maximum expected pulse width value, used for normalization (adjustable based on your sensor)

### 3. HSV Data Structure
```cpp
// ---------- HSV Struct ----------
struct HSV {
  float h; // 0-360
  float s; // 0-1
  float v; // 0-1
};
```
Defines a structure to store HSV (Hue, Saturation, Value) color values:
- **h (Hue)**: Color type ranging from 0-360 degrees
- **s (Saturation)**: Color purity ranging from 0 (gray) to 1 (pure color)
- **v (Value)**: Brightness ranging from 0 (black) to 1 (white)

### 4. Setup Function
```cpp
void setup() {
  // Set S0 - S3 as outputs
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);

  // Set sensor output as input
  pinMode(sensorOut, INPUT);

  // Set frequency scaling to 20%
  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);

  // Start Serial Monitor
  Serial.begin(9600);
  Serial.println("TCS3200 Calibration Started...");
}
```
**Initialization process:**
- **Pin Configuration**: Sets control pins (S0-S3) as outputs and sensor output as input
- **Frequency Scaling**: Sets to 20% by making S0=HIGH and S1=LOW
  - This provides a good balance between sensitivity and response time
- **Serial Communication**: Initializes at 9600 baud for data output

### 5. Main Loop Function
```cpp
void loop() {
  // Read pulse widths
  redPW = getRedPW();
  delay(50);
  greenPW = getGreenPW();
  delay(50);
  bluePW = getBluePW();
  delay(50);

  // Convert to normalized 0-255 RGB values
  float r_norm = normalizePW(redPW);
  float g_norm = normalizePW(greenPW);
  float b_norm = normalizePW(bluePW);

  // Convert to HSV
  HSV color = rgb2hsv(r_norm, g_norm, b_norm);

  // Print RGB and HSV values
  Serial.print("R: "); Serial.print(r_norm);
  Serial.print(" G: "); Serial.print(g_norm);
  Serial.print(" B: "); Serial.print(b_norm);
  Serial.print(" | H: "); Serial.print(color.h);
  Serial.print(" S: "); Serial.print(color.s, 2);
  Serial.print(" V: "); Serial.println(color.v, 2);

  delay(500);
}
```
**Main execution cycle:**
1. **Sequential Color Reading**: Reads each color with 50ms delays to allow sensor stabilization
2. **Normalization**: Converts raw pulse widths to standardized 0-255 RGB values
3. **Color Space Conversion**: Transforms RGB to HSV for better color analysis
4. **Data Output**: Displays both RGB and HSV values via Serial Monitor
5. **Cycle Delay**: 500ms pause before next reading cycle

### 6. Pulse Width Normalization Function
```cpp
float normalizePW(int pw) {
  float val = (MAX_PW - pw) * 255.0 / MAX_PW;
  if (val < 0) val = 0;
  if (val > 255) val = 255;
  return val;
}
```
**Normalization logic:**
- **Inversion**: Higher pulse width = lower light intensity, so we subtract from MAX_PW
- **Scaling**: Maps the result to 0-255 range (standard RGB values)
- **Clamping**: Ensures values stay within valid 0-255 bounds
- **Formula**: `normalized_value = (MAX_PW - pulse_width) * 255 / MAX_PW`

### 7. RGB to HSV Conversion Function
```cpp
HSV rgb2hsv(float r, float g, float b) {
  float rr = r / 255.0;
  float gg = g / 255.0;
  float bb = b / 255.0;
  
  float maxc = max(rr, max(gg, bb));
  float minc = min(rr, min(gg, bb));
  float delta = maxc - minc;

  HSV hsv;

  // Hue calculation
  if (delta == 0) hsv.h = 0;
  else if (maxc == rr) hsv.h = 60 * fmod(((gg - bb) / delta), 6);
  else if (maxc == gg) hsv.h = 60 * (((bb - rr) / delta) + 2);
  else hsv.h = 60 * (((rr - gg) / delta) + 4);
  if (hsv.h < 0) hsv.h += 360;

  // Saturation calculation
  hsv.s = (maxc == 0) ? 0 : delta / maxc;

  // Value calculation
  hsv.v = maxc;

  return hsv;
}
```
**HSV Conversion Algorithm:**
1. **Normalization**: Converts RGB values to 0-1 range
2. **Min/Max/Delta**: Finds the minimum, maximum, and difference of RGB values
3. **Hue Calculation**: 
   - If no color difference (delta=0): Hue = 0
   - Based on which color is dominant, calculates hue in degrees (0-360)
   - Uses different formulas for red, green, or blue dominance
4. **Saturation**: Measures color purity (delta/max)
5. **Value**: Represents brightness (maximum of RGB values)

### 8. Color Reading Functions
```cpp
int getRedPW() {
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  return pulseIn(sensorOut, LOW);
}

int getGreenPW() {
  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);
  return pulseIn(sensorOut, LOW);
}

int getBluePW() {
  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);
  return pulseIn(sensorOut, LOW);
}
```
**Filter Selection Logic:**
- **Red Filter**: S2=LOW, S3=LOW - Activates red-filtered photodiodes
- **Green Filter**: S2=HIGH, S3=HIGH - Activates green-filtered photodiodes  
- **Blue Filter**: S2=LOW, S3=HIGH - Activates blue-filtered photodiodes
- **pulseIn()**: Measures the duration of LOW pulse from sensor output
  - Longer pulse = lower frequency = less light intensity
  - Shorter pulse = higher frequency = more light intensity

### Key Programming Concepts Used

1. **Modular Design**: Separate functions for each color and conversion
2. **Data Structures**: Custom HSV struct for organized data storage
3. **Mathematical Algorithms**: RGB-to-HSV conversion using standard formulas
4. **Hardware Interfacing**: Digital pin control and pulse width measurement
5. **Serial Communication**: Real-time data monitoring and debugging
6. **Calibration System**: Adjustable parameters for different environments

## Calibration

The sensor may need calibration for your specific environment:

1. **Adjust MAX_PW value**:
   - Current value: 2000
   - Test with white/bright objects
   - Increase if readings are too high
   - Decrease if readings are too low

2. **Frequency Scaling**:
   - Currently set to 20% (S0=HIGH, S1=LOW)
   - Can be adjusted based on your requirements:
     - 2%: S0=LOW, S1=LOW
     - 20%: S0=HIGH, S1=LOW
     - 100%: S0=LOW, S1=HIGH

## Troubleshooting

### Common Issues

1. **No readings/All zeros**:
   - Check wiring connections
   - Verify power supply
   - Ensure sensor is not covered

2. **Inconsistent readings**:
   - Improve lighting conditions
   - Reduce ambient light interference
   - Adjust MAX_PW calibration value

3. **Readings too high/low**:
   - Calibrate MAX_PW value
   - Check sensor distance from object
   - Verify frequency scaling settings

### Debug Tips
- Use Serial Monitor to observe raw pulse width values
- Test with known colors (red, green, blue objects)
- Check all pin connections with a multimeter

## Example Output

```
TCS3200 Calibration Started...
R: 200 G: 45 B: 30 | H: 12.5 S: 0.85 V: 0.78
R: 50 G: 180 B: 40 | H: 108.2 S: 0.78 V: 0.71
R: 40 G: 55 B: 190 | H: 234.0 S: 0.79 V: 0.75
```

### Color Interpretation
- **Red objects**: High R value, low G/B values
- **Green objects**: High G value, low R/B values  
- **Blue objects**: High B value, low R/G values
- **HSV values** provide more intuitive color analysis:
  - Hue (H): Color type (0°=Red, 120°=Green, 240°=Blue)
  - Saturation (S): Color purity (0=grayscale, 1=pure color)
  - Value (V): Brightness (0=black, 1=bright)

## Applications

This color sensor system can be used for:
- Color sorting systems
- Object detection and classification
- Quality control in manufacturing
- Robotics navigation
- Art and design projects
- Educational demonstrations

## License

This project is open source and available for educational and non-commercial use.

---

**Course**: CSE461  
**Author**: Aritra Chakraborty  
**Date**: September 2025
