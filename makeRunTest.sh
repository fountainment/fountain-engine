if [ -f "main" ]; then
	rm main
fi

if [ ! -f "libfountain.so" ]; then
	echo miss libfountain.so
	echo you must make the lib first.
	echo trying \"make\".
	make
fi

if [ ! -f "/usr/lib/libfountain.so" ]; then
	echo miss /usr/lib/libfountain.so
	echo you must install the lib first.
	echo trying \"sudo make install\".
	sudo make install
fi

if [ ! -z "`cmp libfountain.so /usr/lib/libfountain.so`" ]; then
	echo make sure you have installed the current lib.
	echo trying \"sudo make install\".
	sudo make install
fi

g++ main.cpp -o main -I./include -I./Box2D -L ./Box2D/Build/bin/Release -lX11 -lGL -lfreeimage -lfountain -lBox2D -s

if [ -f "main" ]; then
	echo compilation finished!
	exec ./main
fi
