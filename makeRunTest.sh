#!/bin/bash

CC="g++"
LIN_LIBS="-lfountain -lX11 -lGL -lGLEW -lfreeimage -lfreetype -lBox2D -lopenal -lRakNet -lpthread"
WIN_LIBS="-lfountain -lgdi32 -lglew_static -lopengl32 -lglu32 -lFreeImage -lfreetype -lBox2D -lOpenAL32 -lRakNet"
CFLAGS="-fexceptions -Wall -g -DDEBUG -I./include -I. -I/usr/include/freetype2 -I./RakNet -L./Box2D/Build/bin/Debug -L. -L./RakNet/bin/Debug"
BOX2D="Box2D/Build/bin/Debug/libBox2D.a"
FONT="resources/font/test.ttc"
WAVE="resources/sound/test.wav"

LIBS=$LIN_LIBS

if [ ! -e `uname|grep MINGW` ]; then
	LIBS=$WIN_LIBS
fi

if [ ! -f $FONT ]; then
	mkdir resources/font
	wget http://www.fountainment.com/download/test.ttc
	mv test.ttc $FONT
fi

if [ ! -f $WAVE ]; then
	mkdir resources/sound
	wget http://www.fountainment.com/download/test.wav
	mv test.wav $WAVE
fi

if [ ! -f $BOX2D ]; then
	cd Box2D/Build
	make config="debug"
	cd ../..
fi

if [ -f "main" ]; then
	rm -f main
fi

make

$CC main.cpp testscene.cpp STG.cpp -o main $CFLAGS $LIBS

if [ -f "main" ]; then
	echo compilation finished!
	exec ./main
fi
