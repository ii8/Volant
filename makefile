
SHELL = /bin/sh

#gcc -Wall -o volant main.c -lm -lGL -lGLEW -lglfw shade.c vmath.c mesh.c log.c camera.c

FLAGS=-Wall -c

all: volant

volant: main.o mesh.o camera.o shade.o log.o vmath.o
	gcc -Wall -o volant main.o vmath.o shade.o mesh.o log.o camera.o -lm -lglfw -lGL -lGLEW

main.o: main.c
	gcc $(FLAGS) main.c

mesh.o: mesh.c
	gcc $(FLAGS) mesh.c

camera.o: camera.c
	gcc $(FLAGS) camera.c

shade.o: shade.c
	gcc $(FLAGS) shade.c

log.o: log.c
	gcc $(FLAGS) log.c

vmath.o: vmath.c
	gcc $(FLAGS) vmath.c


build:
	zenity --warning && gcc -Wall -o volant main.c shade.c vmath.c mesh.c log.c camera.c -lm -lGL -lGLEW -lglfw

clean:
	rm -rf *.o *.gch
