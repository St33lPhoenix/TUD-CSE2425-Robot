/**
 * Student name: <REDACTED>
 * NetID: gbot
 * Student number: 5092094
 */

#define SENSOR_SOUND_TRIG_F PA10
#define SENSOR_SOUND_ECHO_F PB9
#define SENSOR_SOUND_CONVERT_TO_CM 58

bool pingStart = false;
long riseTime = -1;
long distance = -1;

void setup() {
  setupControl();

  // Interrupt
  attachInterrupt(digitalPinToInterrupt(SENSOR_SOUND_ECHO_F), interrupt, CHANGE);
}

void loop() {
  // We are not waiting for an echo currently
  if (!pingStart) {
    pingStart = true;
    triggerUltraSound(SENSOR_SOUND_TRIG_F);
  }

  // Drive
  if (distance < 15) {
    driveForward(0);
  }
  else {
    driveForward(255);
  }
}

// Interrupt to measure pulse length
// Will get called on rising and falling edge
void interrupt() {
  bool high = getDigitalPin(SENSOR_SOUND_ECHO_F);

  // Start of pulse
  if (high) {
    riseTime = micros();
    return;
  }

  long delta = micros() - riseTime;
  distance = delta / SENSOR_SOUND_CONVERT_TO_CM;

  // We can start new pings now
  pingStart = false;
}
