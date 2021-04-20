# MushroomSense
**This project is still a work in progress.** I hope to make it a simple plug and play project eventually.
I hope to have an install wizard in the future so folks don't need to deal with flashing the code from the 
Arduino IDE.

I thought it would be cool to set up a fully automated mushroom farm. Much like animals, 
mushrooms expel CO2 as a byproduct of respiration. As such CO2 levels can be an interesting 
variable to track and manipulate when growing mushrooms. However CO2 sensors are pricey and 
I wanted one that I could integrate into Home Assistant which I planned to use as the brains 
of my automated mushroom farm. MushroomSense is my solution to this problem and as icing on the cake 
it'll measure temperature and humiditiy and having good control over these two varibles is vital to 
growing mushrooms (although if that's all your looking for there are dirt cheep solutions available 
[I've got one of these for example](https://smile.amazon.com/dp/B087313N8F/ref=cm_sw_em_r_mt_dp_8T7RPHPERMNQWD257779)
and its pretty easy to get integrated with home assistant with a bit of google-ing).

## Hardware
The hardware is all available from Adafruit:
* [Assembled Feather HUZZAH w/ ESP8266 WiFi With Stacking Headers](https://www.adafruit.com/product/3213) - $19.95
* [Adafruit FeatherWing OLED - 128x64 OLED Add-on For Feather - STEMMA QT / Qwiic](https://www.adafruit.com/product/4650) -$14.95
* [Adafruit SCD-30 - NDIR CO2 Temperature and Humidity Sensor - STEMMA QT / Qwiic](https://www.adafruit.com/product/4867) - $58.95
* [STEMMA QT / Qwiic JST SH 4-Pin Cable - 200mm Long](https://www.adafruit.com/product/4401) -$0.95
 
These prices could change but the total comes out to about $95. Not too shabby for a wifi enabled CO2 sensor. 
I know the sensor is a bit pricey but that's because its the real deal. The cheaper CO2 sensors measure effective CO2. 
essentially they measure some other gasses and use some math to make an educated guess on how much CO2 there is. A 
future goal is to support one of these cheaper sensors so people can save some money and upgrade later if they wish. 
If your looking to write some this project can be modified to support any sensor you want.

## Arduino IDE Setup 

### Set Up Board 
File -> Preferences

Additional Boards Manager URLs:
`http://arduino.esp8266.com/stable/package_esp8266com_index.json`

Tools -> Board -> Boards Manager

search `esp8266` and install `esp8266 by ESP8266 Comunity`

Tools -> Board

select Adafruit Feather HUZZAH ESP8266

### Set Up Libraries
Tools -> Manage Libraries
* Adafruit_GFX
* Adafruit_SH110X
* Adafruit_SCD30
* ArduinoJson

## Compile and Upload
At this point you should be able to compile and upload the sketch

# Developing Web interface:

`npm run dev`

`npm install html-minifier -g`

then build with `bash build.sh`


