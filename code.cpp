#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// -------------------- LCD --------------------
LiquidCrystal_I2C lcd(0x27, 16, 2);  // change if your scanner showed a different address

// -------------------- Motor Driver Pins --------------------
#define IN1 9
#define IN2 10
#define IN3 11
#define IN4 12

// -------------------- Ultrasonic Pins ----------------------
#define TRIG_PIN 3
#define ECHO_PIN 2

// -------------------- TCS3200 Color Sensor Pins ------------
#define S0 4
#define S1 5
#define S2 6
#define S3 7
#define sensorOut 8

// -------------------- Gas Sensor Pins ----------------------
#define MQ135_DO 13   // Digital Output
#define MQ135_AO A0   // Analog Output
#define CO2ZERO 55    // Calibration offset for ppm

// -------------------- Variables ----------------------------
int redPW, greenPW, bluePW;
const int MAX_PW = 2000;   // adjust after calibration
const float HUE_THRESHOLD = 100.0;  // hue threshold for banana ripeness

// -------------------- HSV Struct ---------------------------
struct HSV {
  float h; // 0–360
  float s; // 0–1
  float v; // 0–1
};

// -------------------- Motor Control ------------------------
void moveForward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void stopCar() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

// -------------------- Ultrasonic ---------------------------
long getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH);
  long distance = duration * 0.034 / 2; // cm
  return distance;
}

// -------------------- Gas Sensor ---------------------------
int readGasPPM() {
  int co2now[10];
  int zzz = 0;
  for (int x = 0; x < 10; x++) {
    co2now[x] = analogRead(MQ135_AO);
    delay(50);
  }
  for (int x = 0; x < 10; x++) zzz += co2now[x];
  int co2raw = zzz / 10;
  int co2ppm = co2raw - CO2ZERO;
  return co2ppm;
}

// -------------------- Color Sensor -------------------------
int getRedPW()  { digitalWrite(S2, LOW);  digitalWrite(S3, LOW);  return pulseIn(sensorOut, LOW); }
int getGreenPW(){ digitalWrite(S2, HIGH); digitalWrite(S3, HIGH); return pulseIn(sensorOut, LOW); }
int getBluePW() { digitalWrite(S2, LOW);  digitalWrite(S3, HIGH); return pulseIn(sensorOut, LOW); }

float normalizePW(int pw) {
  float val = (MAX_PW - pw) * 255.0 / MAX_PW;
  if (val < 0) val = 0;
  if (val > 255) val = 255;
  return val;
}

// -------------------- RGB → HSV ----------------------------
HSV rgb2hsv(float r, float g, float b) {
  float rr = r / 255.0;
  float gg = g / 255.0;
  float bb = b / 255.0;

  float maxc = max(rr, max(gg, bb));
  float minc = min(rr, min(gg, bb));
  float delta = maxc - minc;

  HSV hsv;
  if (delta == 0) hsv.h = 0;
  else if (maxc == rr) hsv.h = 60 * fmod(((gg - bb) / delta), 6);
  else if (maxc == gg) hsv.h = 60 * (((bb - rr) / delta) + 2);
  else hsv.h = 60 * (((rr - gg) / delta) + 4);
  if (hsv.h < 0) hsv.h += 360;

  hsv.s = (maxc == 0) ? 0 : delta / maxc;
  hsv.v = maxc;

  return hsv;
}

// -------------------- Banana Classifier --------------------
String classifyBanana(int ppm, HSV color) {
  // Combine color + gas thresholds
  if (ppm > 400 || color.h >= HUE_THRESHOLD) {
    return "ROTTEN";
  } else {
    return "GOOD";
  }
}

// -------------------- Setup -------------------------------
void setup() {
  // Motor pins
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);

  // Ultrasonic pins
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Color sensor
  pinMode(S0, OUTPUT); pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT); pinMode(S3, OUTPUT);
  pinMode(sensorOut, INPUT);
  digitalWrite(S0, HIGH);  // 20% scaling
  digitalWrite(S1, LOW);

  // Gas sensor
  pinMode(MQ135_DO, INPUT);

  // LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Banana Car Ready");

  // Serial
  Serial.begin(9600);
}

// -------------------- Loop -------------------------------
void loop() {
  long distance = getDistance();

  if (distance > 4) {
    moveForward();
    lcd.setCursor(0,0);
    lcd.print("Moving Forward ");
  } else {
    stopCar();
    lcd.setCursor(0,0);
    lcd.print("Object Detected");

    // ----------- Read sensors -----------
    int ppm = readGasPPM();

    int r_pw = getRedPW();   delay(50);
    int g_pw = getGreenPW(); delay(50);
    int b_pw = getBluePW();  delay(50);

    float r_norm = normalizePW(r_pw);
    float g_norm = normalizePW(g_pw);
    float b_norm = normalizePW(b_pw);

    HSV color = rgb2hsv(r_norm, g_norm, b_norm);

    // ----------- Classify banana --------
    String result = classifyBanana(ppm, color);

    lcd.setCursor(0,1);
    lcd.print("Banana: " + result + "   ");

    Serial.print("Gas PPM: "); Serial.print(ppm);
    Serial.print(" | H: "); Serial.print(color.h);
    Serial.print(" S: "); Serial.print(color.s,2);
    Serial.print(" V: "); Serial.print(color.v,2);
    Serial.print(" | Result: "); Serial.println(result);

    delay(3000); // Show result
  }
}
