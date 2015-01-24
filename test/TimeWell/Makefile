EPATH = ../..
CPPS = TimeWell.cpp scenes.cpp
OBJS = TimeWell.o scenes.o
CC = g++
CFLAGS = -Wall -g -I$(EPATH)/include -I$(EPATH) -I/usr/include/freetype2 -c
LIN_LIBS = -lfountain -lX11 -lGL -lGLEW -lfreeimage -lfreetype -lBox2D -lopenal
WIN_LIBS = -lfountain -lgdi32 -lglew_static -lopengl32 -lglu32 -lFreeImage -lfreetype -lBox2D -lOpenAL32

win: $(OBJS) $(EPATH)/libfountain.a
	$(CC) $(OBJS) -g -L$(EPATH) -L$(EPATH)/Box2D/Build/bin/Release $(WIN_LIBS) -o TimeWell

lin: $(OBJS) $(EPATH)/libfountain.a
	$(CC) $(OBJS) -g -L$(EPATH) -L$(EPATH)/Box2D/Build/bin/Release $(LIN_LIBS) -o TimeWell

TimeWell.o: TimeWell.cpp scenes.h
	$(CC) $(CFLAGS) TimeWell.cpp -o TimeWell.o

scenes.o: scenes.cpp scenes.h
	$(CC) $(CFLAGS) scenes.cpp -o scenes.o

clean:
	rm -rf *.o TimeWell
