
SHELL = /bin/sh

#gcc -Wall -o volant main.c -lm -lGL -lGLEW -lglfw shade.c vmath.c mesh.c log.c camera.c

FLAGS=-Wall -lglfw -lGL -lGLEW

all: volant

volant: main.o mesh.o camera.o shade.o log.o vmath.o
	gcc $(CFLAGS) -o volant main.o vmath.o shade.o mesh.o log.o camera.o

main.o: main.c
	gcc $(CFLAGS) main.c

mesh.o: mesh.c
	gcc $(CFLAGS) mesh.c

camera.o: camera.c
	gcc $(CFLAGS) camera.c

shade.o: shade.c
	gcc $(CFLAGS) shade.c

log.o: log.c
	gcc $(CFLAGS) log.c

vmath.o: vmath.c
	gcc $(CFLAGS) vmath.c -lm

clean:
	rm -rf *o *.gch
