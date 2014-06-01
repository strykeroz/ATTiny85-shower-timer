
// --------------------------------------------------------------------------------
// Drop into deep sleep
//
void bedTimeNow() {
  // no matter what the mode is currently, make sure it wakes from this to hold
  timerState = 1;
  saveValues();
  // lightshow before sleep
  for(int i=0; i<4; i++) digitalWrite(i, LOW);  // all off
  for(int i=3; i>=0; i--) {
    for(int j=3; j>=0; j--) digitalWrite(j, i==j);
    delay(500);
  }
  // low power all the outputs
  for(int i = 0; i<4; i++) {
    digitalWrite(i, LOW);
    pinMode(i, INPUT);
  }
  digitalWrite(buzzerPin, LOW);
  pinMode(buzzerPin, INPUT);
  // and now put this thing to sleep
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);       // sleep mode is set here
  sleep_enable();                            // and activated here
  sleep_mode();                              // and...sleep.
}


