GCC=gcc
GCCC=gcc -c
SRCDIR=src
HEADDIR=include
LIBDIR=lib
 
CFLAGS= -W -Wall $(shell sdl2-config --cflags)
LDFLAGS=-I include -L lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lpthread

 
main:	liste.o 
	GCC $(SRCDIR)/main.c -o bin/prog $(LIBDIR)/liste.o $(LDFLAGS)
 
liste.o: $(SRCDIR)/liste.c
	GCC -o $(LIBDIR)/liste.o -c $(SRCDIR)/liste.c $(LDFLAGS)

clean:
	del $(LIBDIR)\*.o