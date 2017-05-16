CC=gcc
LD=ld
CFLAGS += -g -O0 -Wall -Werror -I$(PWD)/

all: blend

blend: main.o blend_rgb.o blend_argb.o set_alpha.o
	$(CC) main.o blend_rgb.o blend_argb.o set_alpha.o -o blend

main.o: main.c
	$(CC) -c main.c	
	
blend_argb.o: blend_argb.c
	$(CC) -c blend_argb.c

blend_rgb.o: blend_rgb.c
	$(CC) -c blend_rgb.c

set_alpha.o: set_alpha.c
	$(CC) -c set_alpha.c

clean:
	rm *.o blend
