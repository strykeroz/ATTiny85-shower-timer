// --------------------------------------------------------------------------------
// Retrieve saved values from EEPROM, sets the global address value and returns
// true if successful or false as error condition
//
boolean loadValues() {
  // determine if EEPROM has been initialised 
  int datacount = 0;
  for(int i = 0; i < EEPROMmax; i++) if(EEPROM.read(i) != EEPROMblank) datacount++;
  if(datacount != 1) return false;                        // error condition code

    for(int idx=0; idx < EEPROMmax; idx++) {
    if(EEPROM.read(idx) != EEPROMblank) {
      EEPROMidx = idx;                                    // store global index value
      timerState = EEPROM.read(EEPROMidx);                // read value
      return true;                                        // all good
    }
  } // for
}

// --------------------------------------------------------------------------------
// Save new values to EEPROM, overwrite previous values with 0xAA
//
void saveValues() {
  int newIndex = 0;
  if (!firstRun) newIndex = EEPROMidx + 1;
  if(newIndex > EEPROMmax) newIndex = 0;                  // circular buffer
  EEPROM.write(newIndex, timerState);
  // if reset occurs before this next step, data will be lost
  if(!firstRun) EEPROM.write(EEPROMidx, EEPROMblank);    // overwrite previous if not first run after EEPROM init
  EEPROMidx = newIndex;                                  // setup for the next save
  firstRun = false;
}

// --------------------------------------------------------------------------------
// Overwrite all EEPROM locations with with the default value
//
void initialiseEEPROM() {
  for(unsigned int idx=0; idx < EEPROMmax; idx++) EEPROM.write(idx, EEPROMblank);
  firstRun = true;
}

