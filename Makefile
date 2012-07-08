CC=msp430-gcc
CFLAGS=-Os -Wall -g -mmcu=msp430g2211

OBJS=main.c


all: 
        msp430-gcc -mmcu=msp430g2211 -o main.elf $(OBJS)


clean:
        rm -r *.elf

install :
        mspdebug rf2500 "prog main.elf"
