void printString(char* string, uint16_t x, uint16_t y, uint16_t size){
  uint16_t i = 0;
  char c = string[i];
  while(c != '\0'){
    display.drawChar(x+(i*(8*size)), y,  c, SH110X_WHITE, SH110X_BLACK, size);
    i++;
    c = string[i];
  }
}

void drawBeem(uint16_t x){
  if(x >= 32 && x <= 90){
  display.drawBitmap(x, 36, BITMAP_BEEM, 6, 21, SH110X_WHITE);
  }
}
void dualBeem(uint16_t x){
  if(x > 18){
    display.setRotation(3);
    display.drawBitmap(58+x, 7, BITMAP_BEEM, 6, 21, SH110X_WHITE);
    display.setRotation(1);
    display.drawBitmap(58+x, 36, BITMAP_BEEM, 6, 21, SH110X_WHITE);
  }
}

void clearDisplay(){
  display.setCursor(0,0);
  display.clearDisplay();
  display.display();
}
