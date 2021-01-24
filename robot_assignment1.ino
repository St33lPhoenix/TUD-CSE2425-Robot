/**
 * Student name: <REDACTED>
 * NetID: gbot
 * Student number: 5092094
 */

void setup() {
  setupControl();

  // Assignment logic in setup to only run it once

  // Start the robot
  driveForward(150);

  // Run for 3500ms
  delay(3500);

  // Increase speed
  driveForward(255);

  // Run for 3500ms
  delay(3500);

  // Gradual slowdown
  byte b = 255;
  while (b > 0) {
    driveForward(b--);
    delay(20);
  }
}

void loop() {
  // Nothing
}
