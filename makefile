GCC=gcc
GCCC=gcc -c
SRCDIR=src
HEADDIR=include
LIBDIR=lib
 
CFLAGS= -W -Wall $(shell sdl2-config --cflags)
LDFLAGS=-I include -L lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lpthread -lwsock32

objets= liste.o socket.o client.o sound.o
 
main:	$(objets)
	GCC $(SRCDIR)/main.c -o prog $(LIBDIR)/liste.o  $(LIBDIR)/socket.o $(LIBDIR)/client.o $(LIBDIR)/sound.o $(LDFLAGS)
 
client.o:	$(SRCDIR)/client.c
	GCC -o $(LIBDIR)/client.o -c $(SRCDIR)/client.c $(LDFLAGS)
 
socket.o:	$(SRCDIR)/socket.c
	GCC -o $(LIBDIR)/socket.o -c $(SRCDIR)/socket.c $(LDFLAGS)

sound.o: $(SRCDIR)/sound.c
	GCC -o $(LIBDIR)/sound.o -c $(SRCDIR)/sound.c $(LDFLAGS)

liste.o: $(SRCDIR)/liste.c
	GCC -o $(LIBDIR)/liste.o -c $(SRCDIR)/liste.c $(LDFLAGS)

clean:
	del .\$(LIBDIR)\*.o