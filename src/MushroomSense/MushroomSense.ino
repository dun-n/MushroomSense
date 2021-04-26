#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
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
Adafruit_SCD30  scd30;
ESP8266WebServer server(80);
struct { 
  char ssid[32]     = "Bill Wi The Science Fi";
  char password[64] = "80553328";
  uint16_t refreshInterval = 2;
  unsigned int sleepDelay = 0;
  char nodeName[16]     = "MushroomSence";
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
boolean enableSensors = true;
uint16_t currentPage = PAGE_SPLASH;
unsigned long wakeTime = 0;


boolean debug = true; //todo default to false for production

//system setup
void setup() {
  wakeUp();
  initPeripherals();
  // splash handles factory reset
  splash();
  if(debug) Serial.begin(115200);
  Serial.println("");
  initSensors();
  writeEEPROM();
  readEEPROM();
  setupWifi();
  setupServer();
  //setupAccessPoint();
  //setupWifi();
  clearDisplay();

  Serial.print("Measurement Interval: "); 
  Serial.print(scd30.getMeasurementInterval()); 
  Serial.println(" seconds");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  server.begin();
  
}

//main loop
void loop() {
  pageDispach();
  mainUpdate();
  delay(10);
}

void mainUpdate(){
  if(enableSensors) readSensors();
  handleUserInput();
  handleSerialInput();
  server.handleClient();
  yield();
}

boolean isScreenOn(){
  return staticData.sleepDelay == 0 || (millis() - wakeTime) < staticData.sleepDelay;
}

void wakeUp(){
  wakeTime = millis();
}
