CC = avr-gcc
IDIR = libs/headers/icaro

F_CPU = 16000000UL
MCU_TARGET = atmega328p
OPTIMIZE = -O2

CFLAGS_LIBS = -Wall -g2 -gstabs -Os -fpack-struct -fshort-enums -std=gnu99
CFLAGS_LIBS += -funsigned-char -funsigned-bitfields
CFLAGS_LIBS += -DF_CPU=$(F_CPU)
CFLAGS_LIBS += -mmcu=$(MCU_TARGET)
CFLAGS_LIBS += -MMD -MP -c

INCLUDES = -I$(IDIR)

.PHONY: libicaro imu clean

#
# build libraries
#

libs/timer.o: libs/src/icaro/timer/timer.c libs/headers/icaro/timer/timer.h
	$(CC) $(CFLAGS_LIBS) $(OPTIMIZE) -o $@ $< $(INCLUDES)

libicaro: libs/timer.o
	mkdir -p ./libs/build
	avr-ar -r ./libs/build/libicaro.a  $<

#
# build app
#

CFLAGS = -Wall -g2 -gstabs -Os -fpack-struct -fshort-enums -std=gnu99
CFLAGS += -funsigned-char -funsigned-bitfields
CFLAGS += -DF_CPU=$(F_CPU)
CFLAGS += -mmcu=$(MCU_TARGET)
CFLAGS += -MMD -MP -MFsrc/$@.d -MTsrc/$@.d -c -osrc/$@.o src/$@.c

LFLAGS = -Wl,-Map,$@.map 
LFLAGS += -Llibs/build -mmcu=atmega328p -o$@.elf  ./src/$@.o -licaro

imu: src/imu.c libicaro
	$(CC) $(INCLUDES) $(CFLAGS)
	$(CC) $(LFLAGS)

#
# utils
#

read_fuses:
	avrdude -c avrispmkII -p m328p -P usb -B 15.625 -U lfuse:r:-:i -v

clean:
	rm -rf **/*.d **/*.o **/*.a **/*.elf **/*.map
