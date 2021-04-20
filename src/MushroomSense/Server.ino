#include <avr/pgmspace.h>
#include "Webpage.h"

void handle_root() {
  long len = 0;
  //todo for dev only
  server.sendHeader("Access-Control-Allow-Origin", "*");
  if(connecting){
    long len = sizeof(HTML_MAIN);
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.sendHeader("Content-Encoding", "gzip");
    server.send_P(200, "text/html", HTML_CONNECT,len);  
  } else {
    long len = sizeof(HTML_CONNECT);
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
      server.send(200, "text/plain", server.arg("plain"));
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
    for (uint8_t i = 0; i < server.args(); i++) {
      if(server.argName(i).equals("refreshInterval")){
        staticData.refreshInterval = server.arg(i).toInt();
      } else if(server.argName(i).equals("sleepDelay")){
        staticData.sleepDelay = server.arg(i).toInt();
      } else if(server.argName(i).equals("nodeName")){
        server.arg(i).toCharArray(staticData.nodeName,64);
      } else if(server.argName(i).equals("temperatureUnit")){
        server.arg(i).toCharArray(&staticData.temperatureUnit,1);
      }
    }
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
