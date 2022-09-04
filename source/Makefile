# avrdude -c avrisp -p m328p -P usb -b 19200 -U lfuse:r:-:i -v
# lib
# avr-ar -r "libtest.a"  ./test.o 
# avr-objdump -h -S libtest.a  >"libtest.lss"
# avr-size --format=avr --mcu=atmega328p libtest.a

# program
# avr-gcc -I"C:\path\to\my\workspace\test" -Wall -g2 -gstabs -Os -fpack-struct -fshort-enums -std=gnu99 -funsigned-char -funsigned-bitfields 
#	-mmcu=atmega328p -DF_CPU=16000000UL -MMD -MP -MF"program.d" -MT"program.d" -c -o"program.o" "../program.c"
# avr-gcc -Wl,-Map,MainProgram.map -L"C:\path\to\my\workspace\test\Release" -mmcu=atmega328p -o"MainProgram.elf"  ./program.o   -ltest
# avr-objdump -h -S MainProgram.elf  >"MainProgram.lss"
# avr-size --format=avr --mcu=atmega328p MainProgram.elf

# BODLEVEL = DISABLED
# RSTDISBL = [ ]
# DWEN = [ ]
# SPIEN = [X]
# WDTON = [ ]
# EESAVE = [X]
# BOOTSZ = 2048W_3800
# BOOTRST = [ ]
# CKDIV8 = [ ]
# CKOUT = [ ]
# SUT_CKSEL = EXTXOSC_8MHZ_XX_16KCK_14CK_65MS
# 
# EXTENDED = 0xFF (valid)
# HIGH = 0xD1 (valid)
# LOW = 0xFF (valid)

CC = avr-gcc
IDIR = libs/include

F_CPU = 16000000UL
MCU_TARGET     = atmega328p
OPTIMIZE       = -O2

CFLAGS_LIBS = -Wall -g2 -gstabs -Os -fpack-struct -fshort-enums -std=gnu99
CFLAGS_LIBS += -funsigned-char -funsigned-bitfields
CFLAGS_LIBS += -DF_CPU=$(F_CPU)
CFLAGS_LIBS += -mmcu=$(MCU_TARGET)
CFLAGS_LIBS += -MMD -MP -c

INCLUDES = -I$(IDIR)

.PHONY: libs imu

#
# build libraries
#

libs/timer.o: libs/src/timer/timer.c libs/include/timer/timer.h
	$(CC) $(CFLAGS_LIBS) $(OPTIMIZE) -o $@ $< $(INCLUDES)

libicaro: libs/timer.o
	mkdir -p ./build
	avr-ar -r ./build/libicaro.a  $<

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
