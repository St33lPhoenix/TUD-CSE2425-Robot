/**
 * Student name: <REDACTED>
 * NetID: gbot
 * Student number: 5092094
 */

#define SPEED_ENCODER_R PB1

#define SLOT_COUNT 20.0f
#define CIRCUMFERENCE 21.0f
#define SLOTS_PER_CM (SLOT_COUNT / CIRCUMFERENCE)

#define ITERATIONS 100
#define THRESHOLD 1600.0f

// Distance to drive in cm
double distance = 150;
double pulses = SLOTS_PER_CM * distance - 4; // Small error correction to take braking distance into account

// Moving average
int idx = 0;
int last = 0;
int sum = 0;
int arr[ITERATIONS];
double avg = 0;

double lastAvg = 0;
long pings = 0;

void setup() {
  setupControl();

  // Start driving
  driveForward(255);
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
  if (avg > THRESHOLD && lastAvg <= THRESHOLD) {
    pings++;
  }
  
  lastAvg = avg;
  
  // Note that after this number is reached, the robot will stop
  // This means it will no longer increase in value, meaning no overflow will occur
  if (pings >= pulses) {
    driveForward(0);
  }
}
