/**
 * Student name: <REDACTED>
 * NetID: gbot
 * Student number: 5092094
 */

// PINS
#define LED_R PB11 // OUT
#define LED_L PB10 // OUT

#define SENSOR_IR_D0_L PA1 // IN
#define SENSOR_IR_D0_R PA0 // IN

#define SENSOR_SOUND_TRIG_F PA10 // OUT
#define SENSOR_SOUND_ECHO_F PB9 // IN
#define SENSOR_SOUND_TRIG_S PA9 // OUT
#define SENSOR_SOUND_ECHO_S PB8 // IN
#define SENSOR_SOUND_CONVERT_TO_CM 58

#define H_BRIDGE_AIA PA6 // OUT PWM
#define H_BRIDGE_AIB PA8 // OUT PWM
#define H_BRIDGE_BIA PA7 // OUT PWM
#define H_BRIDGE_BIB PA3 // OUT PWM

#define SPEED_ENCODER_L PB13 // IN
#define SPEED_ENCODER_R PB1 // IN

// Dirty / only saves last change
byte speed = 0;
bool direction = true;

// Setup
void setupControl() {
  // LED voltage pins
  // The LEDs are always grounded
  setPinMode(LED_R, false);
  setPinMode(LED_L, false);

  // IR sensor data pins
  setPinMode(SENSOR_IR_D0_L, true);
  setPinMode(SENSOR_IR_D0_R, true);

  // Ultra sound echo and trigger pins
  setPinMode(SENSOR_SOUND_TRIG_F, false);
  setPinMode(SENSOR_SOUND_ECHO_F, true);
  setPinMode(SENSOR_SOUND_TRIG_S, false);
  setPinMode(SENSOR_SOUND_ECHO_S, true);

  // Motor pins
  setPinMode(H_BRIDGE_AIA, false);
  setPinMode(H_BRIDGE_AIB, false);
  setPinMode(H_BRIDGE_BIA, false);
  setPinMode(H_BRIDGE_BIB, false);

  // Just to be sure
  stop();

  // Speed encoder
  setPinMode(SPEED_ENCODER_L, true);
  setPinMode(SPEED_ENCODER_R, true);
}

/*
 * ---------- LED
 */

// Enable/Disable a LED
// Note that the LED's short pin should already be connected to GND
void setLED(int pin, bool b) {
  setDigitalPin(pin, b);
}

// Enable/Disable left LED
void setLeftLED(bool b) {
  setLED(LED_L, b);
}

// Enable/Disable right LED
void setRightLED(bool b) {
  setLED(LED_R, b);
}

/*
 * ---------- Infra Red
 */

// Read the IR sensor value
int getIRValue(int pin) {
  return getAnalogPin(pin);
}

// Read the IR sensor value for the left sensor
int getLeftIRValue() {
  return getIRValue(SENSOR_IR_D0_L);
}

// Read the IR sensor value for the right sensor
int getRightIRValue() {
  return getIRValue(SENSOR_IR_D0_R);
}

/*
 * ---------- Ultra Sound
 */

// Trigger the ultrasound
// This is a blocking operation
void triggerUltraSound(int pinTrig) {
  // Make sure it starts low
  setDigitalPin(pinTrig, false);
  delayMicroseconds(5);

  // 10 microsecond pulse
  setDigitalPin(pinTrig, true);
  delayMicroseconds(10);
  setDigitalPin(pinTrig, false);
}

// Get the distance in cm
// Note that this is a blocking call and has a few delays in between
long getDistance(int pinTrig, int pinEcho) {
  triggerUltraSound(pinTrig);

  // Get pulse length
  long duration = pulseIn(pinEcho, HIGH);

  return duration / SENSOR_SOUND_CONVERT_TO_CM;
}

// Get the distance in cm from the front sensor
long getFrontDistance() {
  getDistance(SENSOR_SOUND_TRIG_F, SENSOR_SOUND_ECHO_F);
}

// Get the distance in cm from the side sensor
long getSideDistance() {
  getDistance(SENSOR_SOUND_TRIG_S, SENSOR_SOUND_ECHO_S);
}

/*
 * ---------- Motor
 */

byte getSpeed() {
  return speed;
}

bool getDirection() {
  return direction;
}

// Set the engine speed
void setEngineSpeed(int pinA, int pinB, bool forward, byte v) {
  setAnalogPin(pinA, forward ? 0 : v);
  setAnalogPin(pinB, forward ? v : 0);

  // We abuse the fact that both motors have the same value when going in a straight line
  speed = v;
  direction = forward;
}

// Set the left engine speed
// Note that this engine's orientation is different from the right engine so we swap the pins
void setLeftEngineSpeed(bool forward, byte v) {
  setEngineSpeed(H_BRIDGE_BIB, H_BRIDGE_BIA, forward, v);
}

// Set the right engine speed
void setRightEngineSpeed(bool forward, byte v) {
  setEngineSpeed(H_BRIDGE_AIA, H_BRIDGE_AIB, forward, v);
}

// Set both engine speeds forwards
void driveForward(byte v) {
  setLeftEngineSpeed(true, v);
  setRightEngineSpeed(true, v);
}

// Set both engine speeds backwards
void driveBackward(byte v) {
  setLeftEngineSpeed(false, v);
  setRightEngineSpeed(false, v);
}

// Stop
void stop() {
  driveForward(0);
}

/*
 * ---------- Speed Encoder
 */

// TODO

// --- PIN CONTROL

void setPinMode(int pin, bool input) {
  pinMode(pin, input ? INPUT : OUTPUT);
}

int getAnalogPin(int pin) {
  return analogRead(pin);
}

// Note that an arduino onlu supports 0-255 according to the documentation so we use a byte here
void setAnalogPin(int pin, byte b) {
  analogWrite(pin, b < 0 ? 0 : (b > 255 ? 255 : b));
}

bool getDigitalPin(int pin) {
  return digitalRead(pin) == HIGH ? true : false;
}

void setDigitalPin(int pin, bool b) {
  digitalWrite(pin, b ? HIGH : LOW);
}
