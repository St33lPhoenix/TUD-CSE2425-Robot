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
long prevDistance = distance;

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

  long distDelta = distance - prevDistance;
  prevDistance = distance;

  // Object is close
  // Nearing 10cm (border);
  if (distance < 13) {
    driveBackward(255);
    return;
  }

  // Object is far
  // Nearing 20cm (border)
  if (distance > 17) {
    driveForward(255);
    return;
  }

  // We are gaining
  if (distDelta < 0) {
    driveForward(getSpeed() - 25);
  }

  // We are losing
  if (distDelta > 0) {
    driveForward(getSpeed() + 25);
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
