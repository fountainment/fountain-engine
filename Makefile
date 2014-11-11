CPPS = ft_time.cpp ft_3dmodel.cpp ft_render.cpp ft_input.cpp ft_data.cpp ft_algorithm.cpp ft_physics.cpp
CC = g++
CFLAGS = -Wall -O2
LIBS = -lX11 -lGL -lfreeimage -lBox2D

libfountain.so:
	$(CC) $(CPPS) -I ./include -L . -fPIC -shared -o libfountain.so $(LIBS)

install: libfountain.so
	cp include/* /usr/include/ -r
	cp libfountain.so /usr/lib/

clean:
	rm -rf *.so
