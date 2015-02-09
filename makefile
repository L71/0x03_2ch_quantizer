
### Project built with AVR gcc 4.3.3, Crosspack 20100115 on Mac OSX. 
### AVR programmer used: AVR ISPmkII.


### Project specs
PROJNAME = quant

CPUTYPE = atmega328p
AVRDUDECPUTYPE = m328p
# CPUTYPE = atmega168
# AVRDUDECPUTYPE = m168

# AVR CPU Fuse settings for this project
# http://www.engbedded.com/fusecalc
# ext crystal
# CPUFUSEFLAGS = -B 10 -U lfuse:w:0xff:m -U hfuse:w:0xd1:m -U efuse:w:0xfc:m
# int 8Mhz clock
CPUFUSEFLAGS = -B 10 -U lfuse:w:0xe2:m -U hfuse:w:0xd4:m -U efuse:w:0xf9:m

# final .elf file name etc.
ELF = $(PROJNAME).elf
HEX = $(PROJNAME).hex

# object file list
OBJS = main.o

include makefile.avrproj


