/**
 * Student name: <REDACTED>
 * NetID: gbot
 * Student number: 5092094
 */

void setup() {
  setupControl();
}

void loop() {
  // Stop if there is an object within 15cm
  // This is a really safe braking distance, at full speed 5cm was not enough
  if (getFrontDistance() < 15) {
    stop();
    return;
  }
  
  // Drive
  driveForward(255);
}
