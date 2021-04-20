
#define BUTTON_UP 0
#define BUTTON_DOWN 1
#define BUTTON_HOLD 2

//ignore inputs till all inputs are pulled high
void initializeInputs(void (*bh)(char, uint16_t),int sel){
  buttonHandler = bh;
  inputReady = false;
  commit = false;
  selection = sel;
}

void initializeInputs(void (*bh)(char, uint16_t)){
  initializeInputs(bh,0);
}

unsigned long throttleDelay = 50;    // the debounce time; increase if the output flickers
unsigned long holdDelay = 200;    

void callButtonHandler(char button, uint16_t state){
  if(isScreenOn()){
    buttonHandler(button, state);
  }
  wakeUp();
}

void handleUserInput(){
    uint16_t a = digitalRead(BUTTON_A);
    uint16_t b = digitalRead(BUTTON_B);
    uint16_t c = digitalRead(BUTTON_C);
    unsigned long currTime = millis(); 
    
    if(inputReady){
      //BUTTON A
      if ((currTime - lastADown) > throttleDelay) {
        if(AState == HIGH && a == LOW){
          AState = LOW;
          lastADown = currTime;
          // key down Event
          if (buttonHandler) {
            callButtonHandler('A', BUTTON_DOWN);
          }
        }
      }
      if ((currTime - lastADown) > holdDelay) {
         if(AState == LOW && a == LOW){
          AState = LOW;
          lastADown = currTime;
          // key down Event
          if (buttonHandler) {
            callButtonHandler('A', BUTTON_HOLD);
          }
        }
      }
      if ((currTime - lastAUp) > throttleDelay) {
        if(AState == LOW && a == HIGH){
          AState = HIGH;
          lastAUp = currTime;
          // key down Event
          if (buttonHandler) {
            callButtonHandler('A', BUTTON_UP);
          }
        }
      }
      
      //BUTTON B
      if ((currTime - lastBDown) > throttleDelay) {
        if(BState == HIGH &&  b == LOW){
          BState = LOW;
          lastBDown = currTime;
          // key down Event
          if (buttonHandler) {
            callButtonHandler('B', BUTTON_DOWN);
          }
        }
      }
      if ((currTime - lastBDown) > holdDelay) {
         if(BState == LOW && b == LOW){
          BState = LOW;
          lastBDown = currTime;
          // key down Event
          if (buttonHandler) {
            callButtonHandler('B', BUTTON_HOLD);
          }
        }
      }
      if ((currTime - lastBUp) > throttleDelay) {
        if(BState == LOW && b == HIGH){
          BState = HIGH;
          lastBUp = currTime;
          // key down Event
          if (buttonHandler) {
            callButtonHandler('B', BUTTON_UP);
          }
        }
      }
      
      //BUTTON C
      if ( (currTime - lastCDown) > throttleDelay) {
        if(CState == HIGH && c == LOW){
          CState = LOW;
          lastCDown = currTime;
          // key down Event
          if (buttonHandler) {
            callButtonHandler('C', BUTTON_DOWN);
          }
        }
      }
      if ((currTime - lastCDown) > holdDelay) {
         if(CState == LOW && c == LOW){
          CState = LOW;
          lastCDown = currTime;
          // key down Event
          if (buttonHandler) {
            callButtonHandler('C', BUTTON_HOLD);
          }
        }
      }
      if ( (currTime - lastCUp) > throttleDelay) {
        if(CState == LOW && c == HIGH){
          CState = HIGH;
          lastCUp = currTime;
          // key down Event
          if (buttonHandler) {
            callButtonHandler('C', BUTTON_UP);
          }
        }
      }
    } else {
      if(a == HIGH && b == HIGH && c == HIGH ){
        AState = HIGH;
        BState = HIGH;
        CState = HIGH;
        inputReady = true;
      }
    }

}
