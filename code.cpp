// ---------- TCS3200 Color Sensor Pins ----------
#define S0 4
#define S1 5
#define S2 6
#define S3 7
#define sensorOut 8

// ---------- Variables ----------
int redPW, greenPW, bluePW;

// ---------- Max Pulse Width for normalization ----------
const int MAX_PW = 2000;  // adjust according to your sensor readings

// ---------- HSV Struct ----------
struct HSV {
  float h; // 0-360
  float s; // 0-1
  float v; // 0-1
};

// ---------- Function Prototypes ----------
HSV rgb2hsv(float r, float g, float b);
int getRedPW();
int getGreenPW();
int getBluePW();
float normalizePW(int pw);

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

// ---------- Normalize pulse width to 0-255 ----------
float normalizePW(int pw) {
  float val = (MAX_PW - pw) * 255.0 / MAX_PW;
  if (val < 0) val = 0;
  if (val > 255) val = 255;
  return val;
}

// ---------- Convert RGB to HSV ----------
HSV rgb2hsv(float r, float g, float b) {
  float rr = r / 255.0;
  float gg = g / 255.0;
  float bb = b / 255.0;
  
  float maxc = max(rr, max(gg, bb));
  float minc = min(rr, min(gg, bb));
  float delta = maxc - minc;

  HSV hsv;

  // Hue
  if (delta == 0) hsv.h = 0;
  else if (maxc == rr) hsv.h = 60 * fmod(((gg - bb) / delta), 6);
  else if (maxc == gg) hsv.h = 60 * (((bb - rr) / delta) + 2);
  else hsv.h = 60 * (((rr - gg) / delta) + 4);
  if (hsv.h < 0) hsv.h += 360;

  // Saturation
  hsv.s = (maxc == 0) ? 0 : delta / maxc;

  // Value
  hsv.v = maxc;

  return hsv;
}

// ---------- Read Pulse Widths ----------
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
