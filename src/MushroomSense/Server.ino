#include <avr/pgmspace.h>
#include "Webpage.h"

void handle_root() {
  long len = 0;
  //todo for dev only
  server.sendHeader("Access-Control-Allow-Origin", "*");
  if(connecting){
    long len = sizeof(HTML_CONNECT);
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.sendHeader("Content-Encoding", "gzip");
    server.send_P(200, "text/html", HTML_CONNECT,len);  
  } else {
    long len = sizeof(HTML_MAIN);
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.sendHeader("Content-Encoding", "gzip");
    server.send_P(200, "text/html", HTML_MAIN ,len); 
  }
}

void handle_hass() {
  long len = sizeof(HTML_HASS);
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Content-Encoding", "gzip");
  server.send_P(200, "text/html", HTML_HASS,len);
}

void handle_rest() {
  long len = sizeof(HTML_REST);
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Content-Encoding", "gzip");
  server.send_P(200, "text/html", HTML_REST,len);
}

void getFont() {
  long len = sizeof(CSS_FONT);
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Content-Encoding", "gzip");
  server.send_P(200, "text/css", CSS_FONT,len);
  
}

void getStyle() {
  long len = sizeof(CSS_STYLES);
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Content-Encoding", "gzip");
  server.send_P(200, "text/css", CSS_STYLES,len);
}


void getNesCss() {
  long len = sizeof(CSS_NES);
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Content-Encoding", "gzip");
  server.send_P(200, "text/css", CSS_NES,len);
}

void getHighlightJS() {
  //todo for dev only
  long len = sizeof(JS_HIGHLIGHT);
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Content-Encoding", "gzip");
  server.send_P(200, "text/javascript", JS_HIGHLIGHT,len);
  
}

void setCredentials_rest(){
  //todo for dev only
  server.sendHeader("Access-Control-Allow-Origin", "*");
  //server.send(200, "text/plain","hello world");
  if(connecting){
    if (server.method() != HTTP_POST) {
      server.send(405, "text/plain", "Method Not Allowed");
    } else {
      for (uint8_t i = 0; i < server.args(); i++) {
        if(server.argName(i).equals("ssid")){
          server.arg(i).toCharArray(staticData.ssid,32);
        } else if(server.argName(i).equals("password")){
          server.arg(i).toCharArray(staticData.password,64);
        }
      }
      //todo check both are set
      currentPage = PAGE_WIFI_CONNECT;
      server.send(200, "text/plain", "Device connecting. You may reconnect to your normal wifi network. After your device is finished connecting to the network you may find its IP address on the status page found in the devices main menu.");
    } 
  } else {
    server.send(404, "text/plain", "Resouce Not Found");
  }
  delay(100);
}



void handle_sensors_rest() {
  char output[1024];
  StaticJsonDocument<1024> doc;
  doc["temperature_C"] = tempC;
  doc["temperature_F"] = ctof(tempC);
  doc["relative_humidity"] = rh;
  doc["co2"] = co2;
  serializeJson(doc, output);
  //todo for dev only
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "application/json",output);
  delay(100);
}

void handle_settings_rest(){
  char output[1024];
  StaticJsonDocument<1024> doc;
  if (server.method() == HTTP_POST) {
    //POST
    Serial.println("GOT POST");
    StaticJsonDocument<1024> input;
    DeserializationError error = deserializeJson(input, server.arg(0));
    // Test if parsing succeeds.
    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.f_str());
      server.send(400, "plain/text","Bad JSON String");
      return;
    }
    if(input.containsKey("refreshInterval")){
      staticData.refreshInterval = input["refreshInterval"];
    }
    if(input.containsKey("sleepDelay")){
      staticData.sleepDelay = input["sleepDelay"];
    }
    if(input.containsKey("nodeName")){
      //Serial.println("GOT NODENAME");
      //memcpy((char *)input["nodeName"],&staticData.nodeName,64);
      const char* nodeName = input["nodeName"];
      //Serial.println(nodeName);
      //Serial.println(strlen(nodeName));
      //strcpy((char *)staticData.nodeName,(char *)nodeName);
      int i;
      for(i = 0; i < 15 && i < strlen(nodeName); i++){
        staticData.nodeName[i] = nodeName[i];
      }
      staticData.nodeName[i] = '\0';
    }
    if(input.containsKey("temperatureUnit")){
      //memcpy((char *)input["temperatureUnit"],&staticData.temperatureUnit,1);
       const char* temperatureUnit = input["temperatureUnit"];
       if(strlen(temperatureUnit) > 0){
        staticData.temperatureUnit = temperatureUnit[0];
       }
    }
    writeEEPROM();
  }
  doc["refreshInterval"] = staticData.refreshInterval;
  doc["sleepDelay"] = staticData.sleepDelay;
  doc["nodeName"] = staticData.nodeName;
  doc["temperatureUnit"] = &staticData.temperatureUnit;
  serializeJson(doc, output);
  //todo for dev only
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "application/json",output);
  delay(100);
}


void setupServer(){
  server.on("/", handle_root);
  server.on("/hass", handle_hass);
  server.on("/rest", handle_rest);
  server.on("/font.css", getFont);
  server.on("/styles.css", getStyle);
  server.on("/nes.css", getNesCss);
  server.on("/highlight.js", getHighlightJS);
  server.on("/api/v1/connect", setCredentials_rest);
  server.on("/api/v1/sensors", handle_sensors_rest);
  server.on("/api/v1/settings", handle_settings_rest);
}
