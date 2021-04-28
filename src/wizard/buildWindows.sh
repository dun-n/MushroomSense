cp ../MushroomSense/MushroomSense.ino.adafruit.bin ./res/MushroomSense.bin
pyinstaller wizard.py --windowed
cp -r ./res ./dist/wizard
cp -r ./dist/wizard ./dist/wizard-windowssh