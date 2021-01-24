/**
 * Student name: <REDACTED>
 * NetID: gbot
 * Student number: 5092094
 */

#define ITERATIONS 100
#define THRESHOLD 2000.0f

// Moving average
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

// 0: Straight
// 1: Turn left
// 2: Turn right
int state = 0;

void setup() {
  setupControl();

  // Start driving
  driveForward(150);
}

void loop() {
  // Moving average
  bool left = getAverageLeft() > THRESHOLD;
  bool right = getAverageRight() > THRESHOLD;

  // Visual indicator (not required)
  setLeftLED(left);
  setRightLED(right);

  // This is far from elegant but it works
  if (state == 0) {
    driveForward(150);
    if (left != right) {
      state = left ? 1 : 2;
    }
  }
  else if (state == 1) {
    setLeftEngineSpeed(true, 0);
    setRightEngineSpeed(true, 150);
    if (right) {
      state = 0;
    }
  }
  else if (state == 2) {
    setLeftEngineSpeed(true, 150);
    setRightEngineSpeed(true, 0);
    if (left) {
      state = 0;
    }
  }
}

// Moving average for the left sensor
double getAverageLeft() {
  sumL -= arrL[idxL];
  lastL = getLeftIRValue();
  arrL[idxL] = lastL;
  sumL += lastL;
  idxL = (idxL + 1) % ITERATIONS;
  avgL = ((double) sumL) / ITERATIONS;
  
  return avgL;
}

// Moving average for the right sensor
double getAverageRight() {
  sumR -= arrR[idxR];
  lastR = getRightIRValue();
  arrR[idxR] = lastR;
  sumR += lastR;
  idxR = (idxR + 1) % ITERATIONS;
  avgR = ((double) sumR) / ITERATIONS;
  
  return avgR;
}
