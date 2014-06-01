#include <EEPROM.h>                                                // used to remember current state in between resets
#include <avr/sleep.h>                                             // used to deep sleep the device
//
/*
  ____  _                           _____ _                     
 / ___|| |__   _____      _____ _ _|_   _(_)_ __ ___   ___ _ __ 
 \___ \| '_ \ / _ \ \ /\ / / _ \ '__|| | | | '_ ` _ \ / _ \ '__|
  ___) | | | | (_) \ V  V /  __/ |   | | | | | | | | |  __/ |   
 |____/|_| |_|\___/ \_/\_/ \___|_|   |_| |_|_| |_| |_|\___|_|   
                                                         v1.1
 
 ATTiny85 shower timer for a 4 minute shower. 
 Single switch brings reset to GND to enable actions.
 
 Controls 4 indicator LEDs and a piezo buzzer.  
 Holds with all LEDs on with first reset, counts down from 4mins on subsequent reset.
 
 After 2mins without activity, the device drops into a deep sleep.  When awoken
 from sleep, holds at 4mins in preparation for a countdown.
 
 Revision history
 1.0  Initial
 1.1  Timing tweaks for final beep
 
 ATTiny85 connections
 Leg  Function
 1    Reset, pull-up resistor to +5V and switch
 2    D3 LED4
 3    D4 piezo buzzer
 4    GND
 5    D0 LED1
 6    D1 LED2
 7    D2 LED3
 8    +5V
 
 attiny85
            reset -+---+- +5V power
         LED4 pb3 -+   +- pb2 LED3
 piezo buzzer pb4 -+   +- pb1 LED2
              GND -+---+- pb0 LED1
 */

// pin definitions
const int buzzerPin = 4;                                            // piezo


// storage constants
// used to 'zero' out unused EEPROM locations
#define EEPROMblank 255                  
// largest EEPROM cell ID (ATTiny85)
#define EEPROMmax 511

// state variables
int timerState = 1;                                                // 0 = waiting on 4, 1 = counting down
boolean firstRun = false;                                          // true if EEPROM has been freshly initialised
int EEPROMidx = 0;                                                 // where to write in EEPROM

// --------------------------------------------------------------------------------
//
void setup() {
  for(int i = 0; i<4; i++) {
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  }
  pinMode(buzzerPin, OUTPUT);

  // extract current state from EEPROM
  if(!loadValues()) {                                           // if there's no value stored there's work to do
    initialiseEEPROM();                                         // setup EEPROM for use by this sketch ongoing
    saveValues();                                               // timerState value & EEPROMidx have default values since EEPROM load failed
  }

  if(!loadValues()) {                                           // EEPROM error state
    while (true) {
      digitalWrite(0, millis() % 1000 < 250);                   // alternate LEDs, forever
      digitalWrite(1, millis() % 1000 > 250 && millis() % 1000 < 500);
      digitalWrite(2, millis() % 1000 > 500 && millis() % 1000 < 750);
      digitalWrite(3, millis() % 1000 > 750);
      delay(200);
    }
  }

  // switch states now
  timerState++;
  if(timerState>1) timerState = 0; 
  saveValues();

  // one ping only vasily
  digitalWrite(buzzerPin, HIGH);
  delay(300);
  digitalWrite(buzzerPin, LOW);

} // setup()

// --------------------------------------------------------------------------------
//
void loop() {
  int BeepCount = 0;
  int minuteCounter = 0;
  unsigned long BeepTimer = 0L;
  for(int i = 0; i<4; i++) digitalWrite(i, HIGH);                // start with all 4 LEDs lit
  if(timerState) {                                               // counting down
    while(true) {
      // flash LED for currently counting down minute
      if(millis() < 60000) digitalWrite(0, millis() % 1000 < 500); // first minute
      if(millis() > 60000 && millis() < 120000) digitalWrite(1, millis() % 1000 < 500); // 2nd minute
      if(millis() > 120000 && millis() < 180000) digitalWrite(2, millis() % 1000 < 500); // 3rd minute
      if(millis() > 180000 && millis() < 240000) digitalWrite(3, millis() % 1000 < 500); // 4th & final minute

      // beep for the number of minutes remaining
      if(millis() / 60000 > minuteCounter) { 
        minuteCounter++;
        BeepCount = 4-minuteCounter;
        BeepTimer = millis();
        digitalWrite(buzzerPin, HIGH);
      }
      if(BeepCount) {
        if(millis() > BeepTimer + 300) {                        // time to toggle buzzer
          digitalWrite(buzzerPin, !digitalRead(buzzerPin));
          BeepTimer = millis();
          if(!digitalRead(buzzerPin)) BeepCount--;              // if we turned it off, that's one less to do now
        }
      }

      if(millis() > 240000) {
        // and if it's all over, 10 seconds of beeps
        BeepTimer = millis();
        while(millis() < BeepTimer + 10000) digitalWrite(buzzerPin, (millis() % 500 < 250));
        // longbeep
        digitalWrite(buzzerPin, HIGH);
        delay(3000);
        digitalWrite(buzzerPin, LOW);
        while(true) {                                             // and hang here, forever.
          if(millis() > BeepTimer + 120000) bedTimeNow();         // unless it's for longer than 2mins, then sleep
        }
      }
    }
  } else {                                                        // holding, ready to countdown on next reset
    while(true) {                                                 // and hang here, forever.
      if(millis() > 120000) bedTimeNow();                         // unless it's for longer than 2mins, then sleep
    }
  } 
}




