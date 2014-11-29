#!/bin/bash

CC="g++"
LIBS="-lfountain -lX11 -lGL -lfreeimage -lBox2D"
CFLAGS="-fexceptions -Wall -O2 -I./include -I. -L./Box2D/Build/bin/Release -L. -s"

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
