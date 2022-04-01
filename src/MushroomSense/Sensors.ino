void printUint16Hex(uint16_t value) {
    Serial.print(value < 4096 ? "0" : "");
    Serial.print(value < 256 ? "0" : "");
    Serial.print(value < 16 ? "0" : "");
    Serial.print(value, HEX);
}

void printSerialNumber(uint16_t serial0, uint16_t serial1, uint16_t serial2) {
    Serial.print("Serial: 0x");
    printUint16Hex(serial0);
    printUint16Hex(serial1);
    printUint16Hex(serial2);
    Serial.println();
}

unsigned long lastDataRead = 0;

/*
 * Called durring setup, code to initialize sensors
 * add code to initialize custom sensors here
 */
void initSensors(){
  // Try to initialize!
  if (scd30.begin()) {
    Serial.println("SCD30 Found!");
    // make sure self Calibration is off
    scd30.selfCalibrationEnabled(false);
    if (scd30.selfCalibrationEnabled()) {
      Serial.println("Self calibration enabled");
    } else {
      Serial.println("Self calibration disabled");
    }
    connectedSensor = 30;
    if (!scd30.setMeasurementInterval(2)){
      Serial.println("Failed to set measurement interval");
    }
    Serial.print("Measurement Interval: "); 
    Serial.print(scd30.getMeasurementInterval()); 
    Serial.println(" seconds");
  } else {
    uint16_t error;
    char errorMessage[256];

    scd4x.begin(Wire);

    // stop potentially previously started measurement
    error = scd4x.stopPeriodicMeasurement();
    if (error) {
        Serial.print("Error trying to execute stopPeriodicMeasurement(): ");
        errorToString(error, errorMessage, 256);
        Serial.println(errorMessage);
    }

    uint16_t serial0;
    uint16_t serial1;
    uint16_t serial2;
    error = scd4x.getSerialNumber(serial0, serial1, serial2);
    if (error) {
        Serial.print("Error trying to execute getSerialNumber(): ");
        errorToString(error, errorMessage, 256);
        Serial.println(errorMessage);
    } else {
        Serial.println("SCD4X Found!");
        printSerialNumber(serial0, serial1, serial2);
        connectedSensor = 40;
    }

    if(staticData.refreshInterval < 5){
      staticData.refreshInterval = 5;
      writeEEPROM();
    }

    // Start Measurement
    if(staticData.refreshInterval < 30){
      error = scd4x.startPeriodicMeasurement();
    } else {
      error = scd4x.startLowPowerPeriodicMeasurement();
    }
    if (error) {
        Serial.print("Error trying to execute startPeriodicMeasurement(): ");
        errorToString(error, errorMessage, 256);
        Serial.println(errorMessage);
    }
  }
}

/*
 * Called durring main loop, code to grab sensor values
 * add code to retreive values from custom sensors here
 */
void readSensors(){
  if(connectedSensor == 30){
      if (scd30.dataReady()){
      Serial.println("===== Data available =====");
   
      if (!scd30.read()){ Serial.println("Error reading sensor data"); return; }
      validReads ++;
      lastDataRead = millis();
      tempC = scd30.temperature;
      rh = scd30.relative_humidity;
      co2 = scd30.CO2;
      Serial.print("TEMP: ");
      Serial.println(tempC);
      Serial.print("RH: ");
      Serial.println(rh);
      Serial.print("CO2: ");
      Serial.println(co2);
      Serial.println("==========================");
  
      if(validReads == 2){
        if (!scd30.setMeasurementInterval(staticData.refreshInterval)){
          Serial.println("Failed to set measurement interval");
        }
        initialized = true;
      }
    }
  } else if (connectedSensor == 40){
    uint16_t error;
    char errorMessage[256];
    uint16_t readyStatus = 0;
    error = scd4x.getDataReadyStatus(readyStatus);
    readyStatus &= 0b0000011111111111;
    if (error) {
          Serial.print("Error trying to execute getDataReadyStatus(): ");
          errorToString(error, errorMessage, 256);
          Serial.println(errorMessage);
    }
    if(readyStatus && refreshIntervalOver()){
        Serial.println("===== Data available =====");
        uint16_t co2_int;
        error = scd4x.readMeasurement(co2_int, tempC, rh);
        co2 = (float) co2_int;
        if (error) {
            Serial.print("Error trying to execute readMeasurement(): ");
            errorToString(error, errorMessage, 256);
            Serial.println(errorMessage);
        } else if (co2 == 0) {
            Serial.println("Invalid sample detected, skipping.");
        } else {
          lastDataRead = millis();
          validReads ++;
          Serial.print("TEMP: ");
          Serial.println(tempC);
          Serial.print("RH: ");
          Serial.println(rh);
          Serial.print("CO2: ");
          Serial.println(co2);
        }
        Serial.println("==========================");
    }
  }
}

boolean refreshIntervalOver(){
  unsigned long timeNow = millis();
  if(timeNow - (staticData.refreshInterval*1000) >= lastDataRead){
    return true;
  } else if(lastDataRead > timeNow){
    //over flow happened this should only happen every 50 days or so of continuios use
    return true;
  }
  return false;
}

void setSensorRefreshInterval(int selection){
  uint16_t error;
  char errorMessage[256];
  staticData.refreshInterval = selection;
  writeEEPROM();
  if(connectedSensor == 30){
    if (!scd30.setMeasurementInterval(staticData.refreshInterval)){
      Serial.println("Failed to set measurement interval");
    }
  } else if (connectedSensor == 40){
    // stop potentially previously started measurement
    error = scd4x.stopPeriodicMeasurement();
    if (error) {
        Serial.print("Error trying to execute stopPeriodicMeasurement(): ");
        errorToString(error, errorMessage, 256);
        Serial.println(errorMessage);
    }
    // restart periodic measurement
    if(staticData.refreshInterval < 30){
      error = scd4x.startPeriodicMeasurement();
    } else {
      error = scd4x.startLowPowerPeriodicMeasurement();
    }
    if (error) {
        Serial.print("Error trying to execute startPeriodicMeasurement(): ");
        errorToString(error, errorMessage, 256);
        Serial.println(errorMessage);
    }
  }
}


void calibrateCO2(int co2Ref){
  if(connectedSensor == 30){
    if (!scd30.forceRecalibrationWithReference(co2Ref)){
      Serial.println("Failed to force recalibration with reference");
    } else {
      Serial.print("Forced Recalibration reference: ");
      Serial.print(scd30.getForcedCalibrationReference());
      Serial.println(" ppm");
    }
  } else {
    Serial.println("Calibration only avalible for SC30");
  }
}

void calibrateTemp(int tespOffset){
  if(connectedSensor == 30){
    if (!scd30.setTemperatureOffset(tespOffset)){ // 19.84 degrees celcius
      Serial.println("Failed to set temperature offset");
    } else {
      Serial.print("Temperature offset: ");
      Serial.print((float)scd30.getTemperatureOffset()/100.0);
      Serial.println(" degrees C");
    }
  } else {
    Serial.println("Calibration only avalible for SC30");
  }
}
