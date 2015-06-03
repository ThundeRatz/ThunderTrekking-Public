MCU=atmega168
TARGET?=avr-
CC=$(TARGET)gcc
SIZE=$(TARGET)size -C --mcu=$(MCU)
OBJCOPY=$(TARGET)objcopy -O ihex -R .eeprom

LFUSE = 0xF7
HFUSE = 0xDF

DBG=-g -O0
OPTIMIZE=-Os -flto
CFLAGS=-Wall -Wextra $(OPTIMIZE) -mmcu=$(MCU)

PREFIX=build/
BINARY=$(PREFIX)potencia

AVRDUDE=avrdude -c usbasp -p $(MCU)

TARGETS:=$(wildcard *.c)
OBJECTS=$(patsubst %.c,%.o,$(TARGETS))
ALL_OBJECTS:=$(wildcard *.o)
# $< = prerequisito, $@ = alvo:
COMPILE=$(CC) -c $(CFLAGS) $< -o $@
LINK=$(CC) $(OBJECTS) $(CFLAGS) -o $@ $(LDFLAGS)
RM=-rm -rf
MKDIR=mkdir -p
MAKEFILE_PATH:=$(abspath $(lastword $(MAKEFILE_LIST)))
SOURCE_DIR:=$(dir $(MAKEFILE_PATH))

.PHONY: all clean install install_eeprom size

all: $(BINARY)

$(BINARY): $(OBJECTS)
	$(MKDIR) $(PREFIX)
	$(LINK)
	$(OBJCOPY) $@ $@.hex

init.o: init.c usart.h
	$(COMPILE)

radio_isr.o: radio_isr.c watchdog.h
	$(COMPILE)

usart.o: usart.c watchdog.h
	$(COMPILE)

main.o: main.c init.h radio_isr.h usart.h mixagem.h watchdog.h
	$(COMPILE)

mixagem.o: mixagem.c watchdog.h
	$(COMPILE)

watchdog.o: watchdog.c watchdog.h
	$(COMPILE)

%.o: %.c
	@echo "********************************************************************"
	@echo "Atenção: Dependências não definidas para $@, usando regras padrão"
	@echo "********************************************************************"
	$(COMPILE)

fuse:
	$(AVRDUDE) -B 10 -U lfuse:w:$(LFUSE):m
	$(AVRDUDE) -B 10 -U hfuse:w:$(HFUSE):m

install:
	$(AVRDUDE) -U flash:w:$(BINARY).hex:i

install_eeprom:
	$(AVRDUDE) -U eeprom:w:$(BINARY)_eeprom.bin

read:
	$(MKDIR) $(PREFIX)
	$(AVRDUDE) -U flash:r:$(BINARY)_read.bin:i
	$(AVRDUDE) -U lfuse:r:$(BINARY)_lfuse.bin:b
	$(AVRDUDE) -U hfuse:r:$(BINARY)_hfuse.bin:b
	$(AVRDUDE) -U eeprom:r:$(BINARY)_eeprom.bin:i

size:
	$(SIZE) $(BINARY)

clean:
	$(RM) $(ALL_OBJECTS) $(PREFIX)
