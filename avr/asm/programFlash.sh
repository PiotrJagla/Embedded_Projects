#!/bin/bash

echo "Flashin file: $1.asm"

avra $1.asm

sudo avrdude -p atmega328p -c arduino -b 115200 -P /dev/ttyACM0 -U flash:w:$1.hex
