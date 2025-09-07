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
