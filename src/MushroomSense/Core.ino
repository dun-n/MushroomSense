
void initPeripherals(){
  EEPROM.begin(512);
  pinMode(BUTTON_A, INPUT_PULLUP);
  pinMode(BUTTON_B, INPUT_PULLUP);
  pinMode(BUTTON_C, INPUT_PULLUP);
}



void initEEPROM(boolean fource){
  struct { 
    char initialized[16] = "INITIALIZED";
    char ssid[32]     = "";
    char password[64] = "";
    uint16_t refreshInterval = 2;
    unsigned int sleepDelay = 0;
    char nodeName[16] = "MushroomSense";
    char temperatureUnit = 'F';
  } staticDataTemp;
  uint addr = 0;
  EEPROM.get(addr,staticDataTemp);
  if(fource || strcmp(staticDataTemp.initialized,"INITIALIZED") != 0){
    writeEEPROM();
  } else {
    readEEPROM();
  }
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
    initEEPROM(true);
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

#define SERIAL_INITIAL 0
#define SERIAL_READ 1
#define SERIAL_PROCESS 2

#define PARSE_CMD 0
#define PARSE_ARGS 1

char serialBuffer[512];
char serialCommand[512];
uint16_t serialBufferLen = 0;
uint16_t serailState = SERIAL_INITIAL;
void handleSerialInput(){
  while(Serial.available() > 0) {
    // read the incoming byte:
    char incomingByte = Serial.read();
    if(serailState == SERIAL_INITIAL && incomingByte == '<'){ //start
        serialBufferLen = 0;
        serailState = SERIAL_READ;
    } else {
      if(incomingByte == '>'){ //end
        serailState = SERIAL_PROCESS;
      } else { // part of the message
        if(incomingByte < 512){
          serialBuffer[serialBufferLen] = incomingByte;
          serialBufferLen++;
        } else {
          //buffer overflow
          Serial.println("Command length > 512 char it was ignored");
          serailState = SERIAL_INITIAL;
        }
      } 
    }
  }
  if(serailState == SERIAL_PROCESS){
    //handle the command
    Serial.println(serialBuffer);
    uint16_t parserIndex = 0;
    uint16_t parserState = PARSE_CMD;
    char* argPoinjters[4] = {serialBuffer,NULL,NULL,NULL};
    uint16_t curArg = 1;
    char cmd[32] = "";
    for(uint16_t i = 0; i < serialBufferLen && curArg < 4; i++){
        if(serialBuffer[i] == ':' || serialBuffer[i] == ','){
          serialBuffer[i] = '\0';
          argPoinjters[curArg] = serialBuffer+((i+1)*sizeof(char));
          curArg++;
        }
    }
    serailState = SERIAL_INITIAL;
    if(strcmp(argPoinjters[0],"CONNECT_WIFI") == 0){
        strcpy(staticData.ssid, argPoinjters[1]);
        strcpy(staticData.password, argPoinjters[2]);
        writeEEPROM();
        setupWifi(false);
    }
  }
}
