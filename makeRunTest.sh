#!/bin/bash

CC="g++"
LIBS="-lfountain -lX11 -lGL -lGLEW -lfreeimage -lfreetype -lBox2D"
CFLAGS="-fexceptions -Wall -O1 -I./include -I. -L./Box2D/Build/bin/Release -L. -s"
BOX2D="Box2D/Build/bin/Release/libBox2D.a"

if [ ! -f $BOX2D ]; then
	cd Box2D/Build
	make config="release"
	cd ../..
fi

if [ -f "main" ]; then
	rm -f main
fi

if [ ! -f "libfountain.a" ]; then
	make
fi

$CC main.cpp -o main $CFLAGS $LIBS 

if [ -f "main" ]; then
	echo compilation finished!
	exec ./main
fi
