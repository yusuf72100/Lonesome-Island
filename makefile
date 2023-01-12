GCC=gcc
GCCC=gcc -c
SRCDIR=src
HEADDIR=include
LIBDIR=lib
 
CFLAGS= -W -Wall $(shell sdl2-config --cflags)
LDFLAGS=-I include -L lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lpthread -lwsock32

 
main:	liste.o socket.o
	GCC $(SRCDIR)/main.c -o bin/prog $(LIBDIR)/liste.o  $(LIBDIR)/socket.o $(LDFLAGS)
 
socket.o:	$(SRCDIR)/socket.c
	GCC -o $(LIBDIR)/socket.o -c $(SRCDIR)/socket.c $(LDFLAGS)

liste.o: $(SRCDIR)/liste.c
	GCC -o $(LIBDIR)/liste.o -c $(SRCDIR)/liste.c $(LDFLAGS)

clean:
	del $(LIBDIR)\*.o