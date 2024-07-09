COMPILE = avr-gcc -Wall -Os -mmcu=atmega328p -DF_CPU=16000000UL

default: compile upload clean

compile:
	$(COMPILE) -c led.c -o led.o 
	$(COMPILE) -c usartUtils.c -o usartUtils.o 
	$(COMPILE) -c ADC_Utils.c -o ADC_Utils.o 
	$(COMPILE) -o led.elf led.o usartUtils.o ADC_Utils.o
	avr-objdump -h -S led.elf > led.lst
	avr-objcopy -j .text -j .data -O ihex led.elf led.hex
	avr-size --format=avr --mcu=atmega328p led.elf

upload:
	sudo avrdude -v -p atmega328p -c arduino -P /dev/ttyACM0 -b 115200 -U flash:w:led.hex:i

clean:
	rm led.elf
	rm led.o
	rm led.hex

    

