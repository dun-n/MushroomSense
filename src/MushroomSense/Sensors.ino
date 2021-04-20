/*
 * Called durring setup, code to initialize sensors
 * add code to initialize custom sensors here
 */
void initSensors(){
  // Try to initialize!
  if (!scd30.begin()) {
    Serial.println("Failed to find SCD30 chip");
  }
  Serial.println("SCD30 Found!");
  if (!scd30.setMeasurementInterval(staticData.refreshInterval)){
    Serial.println("Failed to set measurement interval");
  }
}

/*
 * Called durring main loop, code to grab sensor values
 * add code to retreive values from custom sensors here
 */
void readSensors(){
    if (scd30.dataReady()){
    Serial.println("Data available!");
 
    if (!scd30.read()){ Serial.println("Error reading sensor data"); return; }
    validReads ++;
    tempC = scd30.temperature;
    rh = scd30.relative_humidity;
    co2 = scd30.CO2;

    if(validReads == 2){
      initialized = true;
    }
  }
}

void setSensorRefreshInterval(int selection){
  staticData.refreshInterval = selection;
  writeEEPROM();
  if (!scd30.setMeasurementInterval(staticData.refreshInterval)){
    Serial.println("Failed to set measurement interval");
  }
}
