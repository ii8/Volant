
SHELL=/bin/sh
CC=gcc
CFLAGS=-Wall -g
LDFLAGS=-lm -lGL -lGLEW -lglfw

SOURCES=\
	main.c\
	vmath.c\
	mesh.c\
	log.c\
	camera.c\
	shade.c
OBJECTS=$(SOURCES:.c=.o)

all: volant

volant: $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o volant

clean:
	rm volant $(OBJECTS)
