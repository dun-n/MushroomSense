cp ../MushroomSense/MushroomSense.ino.adafruit.bin ./res/MushroomSense.bin
pyinstaller wizard.py
cp -r ./res ./dist/wizard/res