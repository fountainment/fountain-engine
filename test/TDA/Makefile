EPATH = ../..
CPPS = TDA.cpp scenes.cpp
OBJS = TDA.o scenes.o
CC = g++
CFLAGS = -Wall -I$(EPATH)/include -I$(EPATH) -I/usr/include/freetype2 -c
LIN_LIBS = -lfountain -lX11 -lGL -lGLEW -lfreeimage -lfreetype -lBox2D -lopenal
WIN_LIBS = -lfountain -lgdi32 -lglew_static -lopengl32 -lglu32 -lFreeImage -lfreetype -lBox2D -lOpenAL32

TDAwin: $(OBJS) $(EPATH)/libfountain.a
	$(CC) $(OBJS) -L$(EPATH) -L$(EPATH)/Box2D/Build/bin/Release $(WIN_LIBS) -o TDA

TDAlin: $(OBJS) $(EPATH)/libfountain.a
	$(CC) $(OBJS) -L$(EPATH) -L$(EPATH)/Box2D/Build/bin/Release $(LIN_LIBS) -o TDA 

TDA.o: TDA.cpp scenes.h
	$(CC) $(CFLAGS) TDA.cpp -o TDA.o

scenes.o: scenes.cpp scenes.h
	$(CC) $(CFLAGS) scenes.cpp -o scenes.o

clean:
	rm -rf *.o TDA
