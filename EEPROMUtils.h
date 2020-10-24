#define EEPROMUTILS_H

#define EEPROM_START_C1     EEPROM_STORAGE_SPACE_START
#define EEPROM_START_C2     EEPROM_START_C1 + 1
#define EEPROM_SCORE        EEPROM_START_C1 + 2

void initEEPROM() {

  uint8_t c1 = EEPROM.read(EEPROM_START_C1);
  uint8_t c2 = EEPROM.read(EEPROM_START_C2);

  if (c1 != 'A' || c2 != 'J') {   
      
    EEPROM.update(EEPROM_START_C1, 'A');
    EEPROM.update(EEPROM_START_C2, 'J');
    EEPROM.put(EEPROM_SCORE, (uint16_t)0);
          
  }

}
