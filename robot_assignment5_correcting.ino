/**
 * Student name: <REDACTED>
 * NetID: gbot
 * Student number: 5092094
 */

#define SPEED_ENCODER_R PB1

#define SENSOR_IR_D0_L PA1
#define SENSOR_IR_D0_R PA0

#define SLOT_COUNT 20.0f
#define CIRCUMFERENCE 21.0f
#define SLOTS_PER_CM (SLOT_COUNT / CIRCUMFERENCE)

#define ITERATIONS 100
#define THRESHOLD_SPEED 1600.0f
#define THRESHOLD_LINE 2000.0f

// Distance to drive in cm
double distance = 150;
double pulses = SLOTS_PER_CM * distance - 4; // Small error correction to take braking distance into account

// Moving average
int idx = 0;
int last = 0;
int sum = 0;
int arr[ITERATIONS];
double avg = 0;
int idxL = 0;
int lastL = 0;
int sumL = 0;
int arrL[ITERATIONS];
double avgL = 0;
int idxR = 0;
int lastR = 0;
int sumR = 0;
int arrR[ITERATIONS];
double avgR = 0;

double lastAvg = 0;
long pings = 0;

void setup() {
  setupControl();

  // Start driving
  driveForward(150);
}

void loop() {
  // Moving average
  sum -= arr[idx];
  last = getAnalogPin(SPEED_ENCODER_R);
  arr[idx] = last;
  sum += last;
  idx = (idx + 1) % ITERATIONS;
  avg = ((double) sum) / ITERATIONS;

  // Detect ping
  if (avg > THRESHOLD_SPEED && lastAvg <= THRESHOLD_SPEED) {
    pings++;
  }
  
  lastAvg = avg;
  
  // Note that after this number is reached, the robot will stop
  // This means it will no longer increase in value, meaning no overflow will occur
  if (pings >= pulses) {
    driveForward(0);
    return;
  }

  // Line following
  bool left = getAverageLeft() > THRESHOLD_LINE;
  bool right = getAverageRight() > THRESHOLD_LINE;

  setLeftLED(left);
  setRightLED(right);

  // Both the same so we just continue
  if (left == right) {
    driveForward(150);
  }
  else if (left) {
    setLeftEngineSpeed(true, 150);
    setRightEngineSpeed(true, 200);
  }
  else if (right) {
    setLeftEngineSpeed(true, 200);
    setRightEngineSpeed(true, 150);
  }
}

// Moving average for the left sensor
double getAverageLeft() {
  sumL -= arrL[idxL];
  lastL = getAnalogPin(SENSOR_IR_D0_L);
  arrL[idxL] = lastL;
  sumL += lastL;
  idxL = (idxL + 1) % ITERATIONS;
  avgL = ((double) sumL) / ITERATIONS;
  
  return avgL;
}

// Moving average for the right sensor
double getAverageRight() {
  sumR -= arrR[idxR];
  lastR = getAnalogPin(SENSOR_IR_D0_R);
  arrR[idxR] = lastR;
  sumR += lastR;
  idxR = (idxR + 1) % ITERATIONS;
  avgR = ((double) sumR) / ITERATIONS;
  
  return avgR;
}
