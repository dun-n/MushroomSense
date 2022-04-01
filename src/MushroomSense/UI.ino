
//launch the active page
void pageDispach(){ 
  Serial.println("CurrentPage ");
   Serial.println(currentPage);

  uint16_t timeOutSelection =0;
  for(uint16_t i = 0; i < TIMEOUT_MENU_LENGTH; i++){
    if(TIMEOUT_MAP[i] == staticData.sleepDelay){
      timeOutSelection = i;
      break;  
    }
  }
  uint16_t tempUnitSelection = 0;
  for(uint16_t i = 0; i < TEMP_UNIT_MENU_LENGTH; i++){
    if(*TEMP_UNIT_MENU[i] == staticData.temperatureUnit){
      tempUnitSelection = i;
      break;
    }
  }

  int minRefreshVal = 2;
  if(connectedSensor == 40){minRefreshVal = 5;}
  
  switch(currentPage){
    case PAGE_MAIN_MENU:
      menu(MAIN_MENU, MAIN_MENU_LENGTH, 0);
      switch(selection){
        case 0:
          currentPage = PAGE_HOME;
          Serial.println("PAGE_HOME");
          break;
        case 1:
          currentPage = PAGE_WIFI_CONFIG;
          Serial.println("PAGE_WIFI_CONFIG");
          break;
        case 2:
          currentPage = PAGE_WIFI_CONNECT;
          Serial.println("PAGE_CONNECT_WIFI");
          break;
        case 3:
          currentPage = PAGE_REFRESH_INTERVAL;
          Serial.println("PAGE_WIFI_CONFIG");
          break;
        case 4:
          currentPage = PAGE_SCREEN_TIMEOUT;
          Serial.println("PAGE_WIFI_CONFIG");
          break;
        case 5:
          currentPage = PAGE_TEMP_UNIT;
          Serial.println("PAGE_TEMP_UNIT");
          break;
        case 6:
          currentPage = PAGE_CALIBRATE;
          Serial.println("PAGE_CALIBRATE");
          break;
        case 7:
          currentPage = PAGE_STATUS;
          Serial.println("PAGE_STATUS");
          break;
      }
      break;
    case PAGE_WIFI_CONFIG:
      setupAccessPoint();
      break;
    case PAGE_WIFI_CONNECT:
      setupWifi(true);
      break;
    case PAGE_REFRESH_INTERVAL:
      setNumber("Refresh Interval","s", staticData.refreshInterval,minRefreshVal,120);
      setSensorRefreshInterval(selection);
      currentPage = PAGE_MAIN_MENU;
      break;
    case PAGE_SCREEN_TIMEOUT:
      menu(TIMEOUT_MENU, TIMEOUT_MENU_LENGTH,timeOutSelection);
      staticData.sleepDelay = TIMEOUT_MAP[selection];
      writeEEPROM();
      currentPage = PAGE_MAIN_MENU;
      break;
    case PAGE_TEMP_UNIT:
      menu(TEMP_UNIT_MENU, TEMP_UNIT_MENU_LENGTH, tempUnitSelection);
      switch(selection){
        case 0:
          staticData.temperatureUnit = 'C';
          break;
        case 1:
          staticData.temperatureUnit = 'F';
          break;
      }
      writeEEPROM();
      currentPage = PAGE_MAIN_MENU;
      break;
    case PAGE_STATUS:
      statusPage();
      break;
    case PAGE_CALIBRATE:
      calabratePage();
      break;
    case PAGE_HOME:
    default:
      homePage();
      break;
  }
}

void handleCalabratePageInputs(char button, uint16_t state){
  if(button == 'C' && state == LOW){
    currentPage = PAGE_MAIN_MENU;
  }
}
void calabratePage(){
  initializeInputs(&handleCalabratePageInputs);
  calibrateCO2(400);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setRotation(1);
  while(currentPage == PAGE_CALIBRATE){
      display.clearDisplay();
      if(isScreenOn()){
        display.setCursor(0,0);
        if(connectedSensor == 30){
          display.println("Current CO2 level");
          display.println("set to 400ppm");
        } else {
          display.println("Calibration not avalible");
          display.println("for this sensor.");
        }
        display.fillRect(0, 44, 16, 17, SH110X_WHITE);
        display.drawBitmap(0, 44, BITMAP_BACK_ICON, 16, 17, SH110X_BLACK);
      }
     display.display();
     yield();
     mainUpdate();
  }
}

void handleStatusInputs(char button, uint16_t state){
  if(button == 'C' && state == LOW){
    currentPage = PAGE_MAIN_MENU;
  }
}

void statusPage(){
  initializeInputs(&handleStatusInputs);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setRotation(1);
      while(currentPage == PAGE_STATUS){
      display.clearDisplay();
      if(isScreenOn()){
        display.setCursor(0,0);
        int wifiStatus = WiFi.status();
        if(debug) WiFi.printDiag(Serial);
        display.println("WIFI: ");
        if(wifiStatus == WL_IDLE_STATUS) display.println("Idle");
        else if(wifiStatus == WL_NO_SSID_AVAIL) display.println("No SSID Available");
        else if(wifiStatus == WL_CONNECTED)display.println("Connected"); 
        else if(wifiStatus == WL_CONNECT_FAILED) display.println("Connection Failed");
        else if(wifiStatus == WL_WRONG_PASSWORD) display.println("Wrong Password");
        else if(wifiStatus == WL_DISCONNECTED) display.println("Not Connected");
        else display.println("Unknown");
        display.setTextSize(.5);
        display.println(" ");
        display.setTextSize(1);
        display.println("IP: ");
        display.println(WiFi.localIP());
        display.fillRect(0, 44, 16, 17, SH110X_WHITE);
        display.drawBitmap(0, 44, BITMAP_BACK_ICON, 16, 17, SH110X_BLACK);
      }
      display.display();
      yield();
      mainUpdate();
    }
}

void splash(){
  currentPage = PAGE_SPLASH;
  Serial.println("128x64 OLED FeatherWing test");
  display.begin(0x3C, true); // Address 0x3C default
 
  Serial.println("OLED begun");
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setRotation(1);
  
  
  //display splach screen
  display.drawBitmap(3, 0, BITMAP_SPLASH, 128, 64, SH110X_WHITE);
  display.display();
  // if b button held down for 1 second of this 2 second durration
  // factory reset board 
  uint buttonACount = 0;
  uint buttonCCount = 0;
  for(int i =0; i < 20; i++){
    delay(100);
    if(!digitalRead(BUTTON_A)) buttonACount++;
    if(!digitalRead(BUTTON_C)) buttonCCount++;
    if(buttonCCount > 10) debug = true;
    display.setCursor(64,0);
    if(debug) display.print("Debug Mode");
    display.display();
  }
  if(buttonACount > 10){
    factoryReset();
  }
  currentPage = PAGE_HOME;
}

void handleSetupWifiReportInputs(char button, uint16_t state){
  if(button == 'C' && state == LOW){
    currentPage = PAGE_MAIN_MENU;
  }
}


void setupWifi(boolean report){
  if(strlen(staticData.ssid) > 0 && strlen(staticData.password) > 0){
    lockInput();
    WiFi.begin(staticData.ssid, staticData.password);
    Serial.println("Connecting to WIFI");
    Serial.print("staticData.ssid ");
    Serial.println(staticData.ssid);
    Serial.print("staticData.password ");
    Serial.println(staticData.password);
  
    int beem1 = 0;
    struct Animation animation1;
    animation1.value = &beem1;
    animation1.initialValue = 0;
    animation1.targetValue = 116;
    animation1.duration = 3000;
    animation1.isLoop = true;
    animation1.isActive = true;
    animation1.easingFunction = &easeInQuart;
    animation1.startTime = max(millis()-1500,(unsigned long)0);

    int beem2 = 0;
    struct Animation animation2;
    animation2.value = &beem2;
    animation2.initialValue = 0;
    animation2.targetValue = 116;
    animation2.duration = 3000;
    animation2.isLoop = true;
    animation2.isActive = true;
    animation2.easingFunction = &easeInQuart;
    animation2.startTime = animation1.startTime+1500;
    
    // Wait for connection
    boolean connecting = true;
    while(connecting){  
      display.clearDisplay();
      if(isScreenOn()){
        display.setCursor(0,0);
        printString("Connecting",24, 5, 1);
        printString("to WIFI",36, 15, 1);
        display.drawBitmap(3, 30, BITMAP_SHROOM, 32, 32, SH110X_WHITE);
        display.drawBitmap(96, 34, BITMAP_ROUTER, 32, 32, SH110X_WHITE);
        updateAnimation(&animation1);
        updateAnimation(&animation2);
        drawBeem(beem1);
        drawBeem(beem1+6);
        drawBeem(beem1+12);
        drawBeem(beem2);
        drawBeem(beem2+6);
        drawBeem(beem2+12);
      }
      if(WiFi.status() == WL_CONNECTED){
        connecting = false;
        Serial.println("WIFI Connected ");
      } else if(WiFi.status() == WL_NO_SSID_AVAIL){
        connecting = false;
        Serial.println("Connection failed");
        Serial.println("SSID could not be reached");
      } else if(WiFi.status() == WL_CONNECT_FAILED){
        connecting = false;
        Serial.println("Connection failed");
      } else if(WiFi.status() == WL_WRONG_PASSWORD){
        connecting = false;
        Serial.println("Connection failed");
        Serial.println("Wrong password");
      }
      display.display();
      mainUpdate();
    }
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    unlockInput();
    if(report){
      initializeInputs(&handleSetupWifiReportInputs);
      while(currentPage == PAGE_WIFI_CONNECT){
        display.clearDisplay();
        if(isScreenOn()){
          display.setCursor(0,0);
          if(WiFi.status() == WL_CONNECTED){
            printString("WIFI Connected", 0, 5, 1);
          } else if(WiFi.status() == WL_DISCONNECTED){
            printString("Disconnected", 0, 5, 1);
          } else if(WiFi.status() == WL_NO_SSID_AVAIL){
            printString("Failed", 0, 5, 1);
            printString("SSID Unreachable", 0, 15, 1);
          } else if(WiFi.status() == WL_CONNECT_FAILED){
           printString("Failed", 0, 5, 1);
          } else if(WiFi.status() == WL_WRONG_PASSWORD){
            printString("Failed", 0, 5, 1);
            printString("Wrong Password", 0, 15, 1);
          }else {
            display.println(WiFi.status());
          }
          display.fillRect(0, 44, 16, 17, SH110X_WHITE);
          display.drawBitmap(0, 44, BITMAP_BACK_ICON, 16, 17, SH110X_BLACK);
        }
        display.display();
        yield();
        mainUpdate();
      }
    } else {
      // do nothing we should get dumped back into previos page
    }
  } else {
     initializeInputs(&handleSetupWifiReportInputs);
      while(currentPage == PAGE_WIFI_CONNECT){
        display.clearDisplay();
        if(isScreenOn()){
          display.setCursor(0,0);
          printString("Please Configure", 0, 5, 1);
          printString("WIFI Settings", 0, 15, 1);
          display.fillRect(0, 44, 16, 17, SH110X_WHITE);
          display.drawBitmap(0, 44, BITMAP_BACK_ICON, 16, 17, SH110X_BLACK);
        }
        display.display();
        yield();
        mainUpdate();
      }
  }
}


const char *apssid = "Mushroom Sense"; // The name of the Wi-Fi network that will be created
const char *appassword = "mushlove";   // The password required to connect to it, leave blank for an open network

void handleConnectionInputs(char button, uint16_t state){
  if(button == 'C' && state == LOW){
    currentPage = PAGE_MAIN_MENU;
  }
}

// this has problems on phones!!!!
void setupAccessPoint(){
    initializeInputs(&handleConnectionInputs);
    connecting = true;
    enableSensors = false;
    WiFi.softAP(apssid, appassword);             // Start the access point
    Serial.print("Access Point \"");
    Serial.print(apssid);
    Serial.println("\" started");

    Serial.print("IP address:\t");
    Serial.println(WiFi.softAPIP());         // Send the IP address of the ESP8266 to the computer
    server.begin();
    
    int x = 0;
    struct Animation animation;
    animation.value = &x;
    animation.initialValue = 0;
    animation.targetValue = 80;
    animation.duration = 4000;
    animation.isLoop = true;
    animation.isActive = true;
    animation.easingFunction = &easeInOutExpo;
    animation.startTime = max(millis()-750,(unsigned long)0);

    int x2 = 0;
    struct Animation animation2;
    animation2.value = &x2;
    animation2.initialValue = 0;
    animation2.targetValue = 80;
    animation2.duration = 4000;
    animation2.isLoop = true;
    animation2.isActive = true;
    animation2.easingFunction = &easeInOutExpo;
    animation2.startTime = animation.startTime+2000;
    
    while(currentPage == PAGE_WIFI_CONFIG){
      display.clearDisplay();
      if(isScreenOn()){
        display.setCursor(0,0);
        display.print("WIFI: ");
        display.println(apssid);
        display.print("Password: ");
        display.println(appassword);
        display.print("http://");
        display.println(WiFi.softAPIP());
        display.drawBitmap(50, 30, BITMAP_SHROOM, 32, 32, SH110X_WHITE);
        updateAnimation(&animation);
        updateAnimation(&animation2);
        dualBeem(x);
        dualBeem(x+6);
        dualBeem(x+12);
        dualBeem(x2);
        dualBeem(x2+6);
        dualBeem(x2+12);
        display.fillRect(0, 44, 16, 17, SH110X_WHITE);
        display.drawBitmap(0, 44, BITMAP_BACK_ICON, 16, 17, SH110X_BLACK);
      }
      display.display();
      yield();
      mainUpdate();
    }
    writeEEPROM();
    server.stop();
    connecting = false;
    enableSensors = true;
}


void handlePromptButtons(char button, uint16_t state){
  if(state == LOW){
    if(button == 'A'){
      selection ++;
    } else if(button == 'B'){
      commit = true;
    } else if(button == 'C'){
      selection --;
    }
  }
}

boolean prompt(char* text){
  currentPage = PAGE_PROMPT;
  initializeInputs(&handlePromptButtons);
  while(!commit){
    display.clearDisplay();
    if(isScreenOn()){
      display.setCursor(24, 0);
      display.print(text);
      display.fillRect(0, 0, 16, 64, SH110X_WHITE);
      display.drawBitmap(0, 3, BITMAP_UP_ICON, 16, 17, SH110X_BLACK);
      display.drawBitmap(0, 24, BITMAP_CHECK_ICON, 16, 17, SH110X_BLACK);
      display.setRotation(3);
      display.drawBitmap(112, 3, BITMAP_UP_ICON, 16, 17, SH110X_BLACK);
      display.setRotation(1);
      
      display.setTextSize(2);
      if(selection % 2 == 0){
        display.fillRect(50, 16, 38, 18, SH110X_WHITE);
        display.setTextColor(SH110X_BLACK);
      } else {
        display.setTextColor(SH110X_WHITE);
      }
      display.setCursor(52, 18);
      display.print("Yes");
  
      if(abs(selection) % 2 == 1){
        display.fillRect(50, 38, 26, 18, SH110X_WHITE);
        display.setTextColor(SH110X_BLACK);
      } else {
        display.setTextColor(SH110X_WHITE);
      }
      display.setCursor(52, 40);
      display.print("No");
      display.display();
      display.setTextColor(SH110X_WHITE);
      display.setTextSize(1);
    }
    display.display();
    mainUpdate();
  }
  return selection % 2 == 0;
}

void handleHomeButtons(char button, uint16_t state){
  Serial.println("HOME Input");
  if(button == 'B' && state == LOW){
    Serial.println("MENU");
    currentPage = PAGE_MAIN_MENU;
  }
}

void homePage(){
  initializeInputs(&handleHomeButtons);
  while(currentPage == PAGE_HOME){
    display.clearDisplay();
    if(isScreenOn()){
      display.fillRect(0, 0, 16, 64, SH110X_WHITE);
      display.drawBitmap(0, 24, BITMAP_MENU_ICON, 16, 17, SH110X_BLACK);
  
      display.setCursor(24, 15);
      display.print("Temp: ");
      display.print(getDisplayTemp(tempC));
      display.print(" ");
      display.print(staticData.temperatureUnit);
  
      display.setCursor(24, 25);
      display.print("RH: ");
      display.print(rh);
      display.print(" %");
  
      display.setCursor(24, 35);
      display.print("CO2: ");
      display.print(co2, 3);
      display.print(" ppm");
    }
    display.display();
    mainUpdate();
  }
}

uint16_t currentMenuLength = 0;
uint16_t currentMin = 0;
void handleMenuButtons(char button, uint16_t state){
  if(state == LOW){
    if(button == 'A'){
      if(selection == 0){
        selection = currentMenuLength - 1;
        if(((int)currentMenuLength) - 6 < 0){
          currentMin = 0;
        } else {
          currentMin = currentMenuLength - 6;
        }
      } else{
        selection--;
        if(selection < currentMin){
          currentMin--;
        }
      }
    } else if(button == 'B'){
      commit = true;
    } else if(button == 'C'){
      if(selection == currentMenuLength -1){
        selection = 0;
        currentMin = 0;
      } else {
        selection ++;
        if(selection == currentMin + 6){
          currentMin++;
        }
      }
    }
  }
}

void menu(char** menu, uint16_t len, int sel){
  currentMin = 0;
  if(sel >= 6){
    currentMin = min(len-6,sel);
  }
  currentMenuLength = len;
  initializeInputs(&handleMenuButtons, sel);
  while(!commit){
    display.clearDisplay();
    if(isScreenOn()){
      display.fillRect(0, 0, 16, 64, SH110X_WHITE);
      display.drawBitmap(0, 3, BITMAP_UP_ICON, 16, 17, SH110X_BLACK);
      display.drawBitmap(0, 24, BITMAP_CHECK_ICON, 16, 17, SH110X_BLACK);
      display.setRotation(3);
      display.drawBitmap(112, 3, BITMAP_UP_ICON, 16, 17, SH110X_BLACK);
      display.setRotation(1);
      for(uint16_t i = 0; i < 6; i++){
        uint16_t curr = i+currentMin;
        if(curr < currentMenuLength){
          if(selection == curr){
            display.fillRect(21, i*10, 116, 11, SH110X_WHITE);
            display.setTextColor(SH110X_BLACK);
          } else {
            display.setTextColor(SH110X_WHITE);
          }
          display.setCursor(24, i*10+2);
          display.println(menu[curr]);
        }
      }
    }
    display.display();
    mainUpdate();
  }
 
}

int getNumChars(int value){
    int n = value;
    int numChars = 0;
    while (n != 0) {
        n /= 10;     // n = n/10
        numChars++;
    }
    //handle if this is has "-" or is 0
    if(selection <= 0) numChars++;
    return numChars;
}

int maxNumber = 100;
int minNumber = 0;
struct Animation animation;
struct Animation unitAnimation;
int currChars = 1;
boolean hold = false;
boolean bounce = false;
void handleSetNumberButtons(char button, uint16_t state){
  if(state == BUTTON_DOWN || state == BUTTON_HOLD){
    if(button == 'A'){
      if(selection < maxNumber){
        selection ++;
        int numChars = getNumChars(selection);
        if(currChars != numChars){
          unitAnimation.initialValue = currChars*12;
          unitAnimation.targetValue = numChars*12;
          unitAnimation.easingFunction = &easeOutExpo;
          unitAnimation.startTime = millis();
          unitAnimation.isActive = true;
          currChars = numChars;
        }
        if(state == BUTTON_HOLD && selection < maxNumber){
          hold = true;
          animation.easingFunction = &linear;
          animation.duration = 200;
        } else{
          animation.easingFunction = &easeOutBounce;
          animation.duration = 750;
        }
        animation.initialValue = -16;
        animation.targetValue = 24;
        animation.startTime = millis();
        animation.isActive = true;
      }
    } else if(button == 'C'){
      if(selection > minNumber){
        selection--;
        int numChars = getNumChars(selection);
        if(currChars != numChars){
          unitAnimation.initialValue = currChars*12;
          unitAnimation.targetValue = numChars*12;
          unitAnimation.easingFunction = &easeInExpo;
          unitAnimation.startTime = millis();
          unitAnimation.isActive = true;
          currChars = numChars;
        }
        if(state == BUTTON_HOLD && selection > minNumber){
          hold = true;
          animation.easingFunction = &linear;
          animation.duration = 200;
        } else{
          animation.easingFunction = &easeOutBounce;
          animation.duration = 750;
        }
        animation.initialValue = 64;
        animation.targetValue = 24;
        animation.startTime = millis();
        animation.isActive = true;
      }
    }
  }
  if(state == BUTTON_DOWN){
    if(button == 'B'){
      commit = true;
    } 
  }
  if(state == BUTTON_UP && hold == true){
      hold = false;
      bounce = true;
  }
}

void setNumber(char* title, char* unit, uint16_t value, int minNum, int maxNum){
  maxNumber = maxNum;
  minNumber = minNum;
  int y = 24;
  animation.value = &y;
  animation.initialValue = 24;
  animation.targetValue = 24;
  animation.duration = 750;
  animation.isLoop = false;
  animation.isActive = false;
  animation.easingFunction = &easeOutBounce;

  currChars = getNumChars(value);
  int space = currChars*12;
  unitAnimation.value = &space;
  unitAnimation.initialValue = space;
  unitAnimation.targetValue = space ;
  unitAnimation.duration = 200;
  unitAnimation.isLoop = false;
  unitAnimation.isActive = true;
  unitAnimation.easingFunction = &easeInExpo;
  animation.startTime = millis();
  
  initializeInputs(&handleSetNumberButtons);
  selection = value;
  while(!commit){
    display.clearDisplay();
    if(isScreenOn()){
      display.fillRect(0, 0, 16, 64, SH110X_WHITE);
      display.drawBitmap(0, 3, BITMAP_UP_ICON, 16, 17, SH110X_BLACK);
      display.drawBitmap(0, 24, BITMAP_CHECK_ICON, 16, 17, SH110X_BLACK);
      display.setRotation(3);
      display.drawBitmap(112, 3, BITMAP_UP_ICON, 16, 17, SH110X_BLACK);
      display.setRotation(1);
      display.setTextSize(2);
      updateAnimation(&animation);
      display.setCursor(60, y-40);
      display.println(selection+1);
      display.setCursor(60, y);
      display.println(selection);
      display.setCursor(60, y+40);
      display.println(selection-1);
      display.setTextSize(1);
      if(bounce){
        animation.easingFunction = &easeOutBounce;
        animation.duration = 750;
        animation.startTime = millis()-300;
        animation.isActive = true;
        bounce = false;
      }
      updateAnimation(&unitAnimation);
      display.setCursor((62+space), 30);
      display.println(unit);
      display.fillRect(16, 0, 128, 10, SH110X_BLACK);
      display.setCursor(24, 0);
      display.println(title);
    }
    display.display();
    mainUpdate();
  }
  currentPage = PAGE_HOME;
}
