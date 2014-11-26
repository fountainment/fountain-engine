CPPS = ft_time.cpp ft_3dmodel.cpp ft_render.cpp ft_input.cpp ft_data.cpp ft_algorithm.cpp ft_physics.cpp ft_scene.cpp
OBJS = ft_time.o ft_3dmodel.o ft_render.o ft_input.o ft_data.o ft_algorithm.o ft_physics.o ft_scene.o
CC = g++
CFLAGS = -Wall -fexceptions -fPIC -Iinclude -I. -c

libfountain.a: $(OBJS)
	rm -f libfountain.a
	ar -r -s libfountain.a $(OBJS)

ft_time.o: ft_time.cpp
	$(CC) $(CFLAGS) ft_time.cpp

ft_3dmodel.o: ft_3dmodel.cpp
	$(CC) $(CFLAGS) ft_3dmodel.cpp

ft_render.o: ft_render.cpp
	$(CC) $(CFLAGS) ft_render.cpp

ft_input.o: ft_input.cpp
	$(CC) $(CFLAGS) ft_input.cpp

ft_data.o: ft_data.cpp
	$(CC) $(CFLAGS) ft_data.cpp

ft_algorithm.o: ft_algorithm.cpp
	$(CC) $(CFLAGS) ft_algorithm.cpp

ft_physics.o: ft_physics.cpp
	$(CC) $(CFLAGS) ft_physics.cpp

ft_scene.o: ft_scene.cpp
	$(CC) $(CFLAGS) ft_scene.cpp

clean:
	rm -rf *.o libfountain.a
