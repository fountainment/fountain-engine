#!/bin/bash

CC="g++"
LIN_LIBS="-lfountain -lX11 -lGL -lGLEW -lfreeimage -lfreetype -lBox2D"
WIN_LIBS="-lfountain -lgdi32 -lglew_static -lopengl32 -lglu32 -lFreeImage -lfreetype -lBox2D"
CFLAGS="-fexceptions -Wall -O1 -I./include -I. -I/usr/include/freetype2 -L./Box2D/Build/bin/Release -L. -s"
BOX2D="Box2D/Build/bin/Release/libBox2D.a"

LIBS=$LIN_LIBS

if [ ! -e `uname|grep MINGW` ]; then
	LIBS=$WIN_LIBS
fi

if [ ! -f $BOX2D ]; then
	cd Box2D/Build
	make config="release"
	cd ../..
fi

if [ -f "main" ]; then
	rm -f main
fi

make

$CC main.cpp -o main $CFLAGS $LIBS 

if [ -f "main" ]; then
	echo compilation finished!
	exec ./main
fi
