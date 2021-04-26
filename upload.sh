PORT=$1
if [ ! ${PORT} ]; then
    PORT=/dev/ttyUSB0
fi
esptool.py --port /dev/ttyUSB0 write_flash 0x0000 MushroomSense.bin