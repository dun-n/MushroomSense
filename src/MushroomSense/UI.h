#define PAGE_SPLASH  0
#define PAGE_HOME  1
#define PAGE_MAIN_MENU 2
#define PAGE_WIFI_CONFIG  3
#define PAGE_WIFI_CONNECT  4
#define PAGE_PROMPT  5
#define PAGE_REFRESH_INTERVAL  6
#define PAGE_SCREEN_TIMEOUT  7
#define PAGE_TEMP_UNIT  8
#define PAGE_STATUS  9
#define PAGE_CALIBRATE  10

uint16_t MAIN_MENU_LENGTH = 8;
char* MAIN_MENU[8] = {
  "Exit",
  "Configure WIFI",
  "Connect WIFI",
  "Refresh Interval",
  "Screen Timeout",
  "Temperature Unit",
  "CO2 Calibration",
  "Status"
};


uint16_t TIMEOUT_MENU_LENGTH = 6;
char* TIMEOUT_MENU[6] = {
  "Never",
  "10 Seconds",
  "30 Seconds",
  "1 Minute",
  "5 Minutes",
  "10 Minutes",
};
unsigned int TIMEOUT_MAP[6] = {
  (unsigned int)0,
  (unsigned int)10000,
  (unsigned int)30000,
  (unsigned int)60000,
  (unsigned int)300000,
  (unsigned int)600000
};

uint16_t TEMP_UNIT_MENU_LENGTH = 2;
char* TEMP_UNIT_MENU[2] = {
  "C",
  "F"
};
