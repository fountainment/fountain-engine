#GLPATH=/usr/include/nvidia-331
GLPATH=/usr/include
CPPS = ft_time.cpp ft_3dmodel.cpp ft_render.cpp ft_input.cpp ft_data.cpp ft_algorithm.cpp ft_physics.cpp ft_scene.cpp
OBJS = ft_time.o ft_3dmodel.o ft_render.o ft_input.o ft_data.o ft_algorithm.o ft_physics.o ft_scene.o
CC = g++
CFLAGS = -Wall -fexceptions -Iinclude -I. -c

libfountain.a: $(OBJS)
	rm -f libfountain.a
	ar -r -s libfountain.a $(OBJS)

ft_time.o: ft_time.cpp
	$(CC) -O2 $(CFLAGS) ft_time.cpp

ft_3dmodel.o: ft_3dmodel.cpp
	$(CC) -O2 -I$(GLPATH) $(CFLAGS) ft_3dmodel.cpp

ft_render.o: ft_render.cpp
	$(CC) -O2 -I$(GLPATH) $(CFLAGS) ft_render.cpp

ft_input.o: ft_input.cpp
	$(CC) -O2 $(CFLAGS) ft_input.cpp

ft_data.o: ft_data.cpp
	$(CC) -O2 $(CFLAGS) ft_data.cpp

ft_algorithm.o: ft_algorithm.cpp
	$(CC) -O2 $(CFLAGS) ft_algorithm.cpp

ft_physics.o: ft_physics.cpp
	$(CC) -O1 $(CFLAGS) ft_physics.cpp

ft_scene.o: ft_scene.cpp
	$(CC) -O2 $(CFLAGS) ft_scene.cpp

clean:
	rm -rf *.o libfountain.a
