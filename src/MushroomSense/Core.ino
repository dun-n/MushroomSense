
void initPeripherals(){
  EEPROM.begin(512);
  pinMode(BUTTON_A, INPUT_PULLUP);
  pinMode(BUTTON_B, INPUT_PULLUP);
  pinMode(BUTTON_C, INPUT_PULLUP);
}

void readEEPROM(){
  uint addr = 0;
  EEPROM.get(addr,staticData);
}

void writeEEPROM(){
  uint addr = 0;
  EEPROM.put(addr,staticData);
  EEPROM.commit(); 
}

//reset the processor 
void(* resetFunc) (void) = 0; //declare reset function @ address 0
//clear EEPROM reset the processor
void factoryReset(){
  //todo replace prompt
  if(prompt("Factory Reset?")){
    display.clearDisplay();
    display.setCursor(0,0);
    display.print("Facotry Rest");
    display.display();
    display.print(".");
    display.display();
    uint addr = 0;
    strncpy(staticData.ssid,"",32);
    display.print(".");
    display.display();
    strncpy(staticData.password,"",64);
    display.print(".");
    display.display();
    EEPROM.put(addr,staticData);
    display.print(".");
    display.display();
    EEPROM.commit(); 
    display.print(".");
    display.display();
    resetFunc();
  }
}

void debugLog(char* message){
  if(debug){
    Serial.println(message);
  }
}

float ctof(float c){
  return (c * 9.0/5.0) + 32.0;
}

float getDisplayTemp(float temp){
  if(staticData.temperatureUnit == 'F'){
    return ctof(temp);
  }
  return temp;
}
