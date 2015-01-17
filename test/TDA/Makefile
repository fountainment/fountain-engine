EPATH = ../..
CPPS = TDA.cpp scenes.cpp
OBJS = TDA.o scenes.o
CC = g++
CFLAGS = -Wall -I$(EPATH)/include -I$(EPATH) -I/usr/include/freetype2 -c
LIBS = -L$(EPATH) -L$(EPATH)/Box2D/Build/bin/Release -lfountain -lX11 -lGLEW -lGL -lfreeimage -lfreetype -lBox2D -lopenal

TDA: $(OBJS) $(EPATH)/libfountain.a
	$(CC) $(OBJS) $(LIBS) -o TDA

TDA.o: TDA.cpp scenes.h
	$(CC) $(CFLAGS) TDA.cpp -o TDA.o

scenes.o: scenes.cpp scenes.h
	$(CC) $(CFLAGS) scenes.cpp -o scenes.o

clean:
	rm -rf *.o TDA
