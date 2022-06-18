#include <SPI.h>
#include <SensirionI2CScd4x.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>        // Include the mDNS library
#include "Bitmap.h"
#include "Input.h"
#include "UI.h"
// Basic demo for readings from Adafruit SCD30
#include <Adafruit_SCD30.h>
#include <ArduinoJson.h>
#include <EEPROM.h>
#include <avr/pgmspace.h>

//global variables 
Adafruit_SH110X display = Adafruit_SH110X(64, 128, &Wire);
short connectedSensor = 0;
Adafruit_SCD30  scd30;
SensirionI2CScd4x scd4x;
ESP8266WebServer server(80);
struct { 
  char initialized[16] = "INITIALIZED";
  char ssid[32] = "";
  char password[64] = "";
  uint16_t refreshInterval = 2;
  unsigned int sleepDelay = 0;
  char nodeName[16] = "MushroomSense";
  char temperatureUnit = 'F';
} staticData;

float tempC = 0;
float tempF = 0;
float rh = 0;
float co2 = 0;
boolean connecting = false;
boolean initialized = false;
uint16_t validReads = 0;

void (*buttonHandler)(char, uint16_t) = NULL; //set to function that takes a char A,B,C and uint16_t with HIGH or LOW

//can be reused for other menus
int selection = 0;
boolean commit = false;

boolean _inputLock = false;
uint16_t AState = HIGH;   
uint16_t BState = HIGH; 
uint16_t CState = HIGH; 
boolean ATriggered = false;
boolean BTriggered = false;
boolean CTriggered = false;
unsigned long lastADown = 0;  // the last time the output pin was toggled
unsigned long lastBDown = 0;  // the last time the output pin was toggled
unsigned long lastCDown = 0;  // the last time the output pin was toggled
unsigned long lastAUp = 0;  // the last time the output pin was toggled
unsigned long lastBUp = 0;  // the last time the output pin was toggled
unsigned long lastCUp = 0;  // the last time the output pin was toggled


boolean inputReady = false;
boolean setupComplete = false;
boolean enableSensors = true;
uint16_t currentPage = PAGE_SPLASH;
unsigned long wakeTime = 0;


boolean debug = false; //todo default to false for production

//system setup
void setup() {
  wakeUp();
  initPeripherals();
  // splash handles factory reset
  splash();
  Serial.begin(115200);
  Serial.println("");
  if(debug) Serial.setDebugOutput(true);
  initEEPROM(false);
  initSensors();
  setupWifi(false);
  setupServer();
  clearDisplay();
  
  server.begin();
  setupComplete = true;
}

//main loop
void loop() {
  pageDispach();
  mainUpdate();
  delay(10);
}

void mainUpdate(){
  MDNS.update();
  if(setupComplete && enableSensors) readSensors();
  handleUserInput();
  if(setupComplete) handleSerialInput();
  if(setupComplete) server.handleClient();
  yield();
}

boolean isScreenOn(){
  unsigned long timeNow = millis();
  if(timeNow < wakeTime){
    // the counter restet (happens after 50 days) we'll wake up to reset wakeTime
    wakeUp();
  }
  return staticData.sleepDelay == 0 || (timeNow - wakeTime) < staticData.sleepDelay;
}

void wakeUp(){
  wakeTime = millis();
}
