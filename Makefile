OBJS = main.o ft_time.o ft_3dmodel.o ft_render.o ft_input.o ft_data.o ft_algorithm.o
LIBS = -lGL -lfreeimage -lX11
CC = g++
CFLAGS = -Wall -O
fountain: $(OBJS)
	$(CC) $(OBJS) -o fountain $(LIBS)

main.o: main.cpp fountain.h
	$(CC) $(CFLAGS) -c main.cpp -o main.o

ft_time.o: ft_time.cpp ft_time.h
	$(CC) $(CFLAGS) -c ft_time.cpp -o ft_time.o

ft_3dmodel.o: ft_3dmodel.cpp ft_3dmodel.h
	$(CC) $(CFLAGS) -c ft_3dmodel.cpp -o ft_3dmodel.o

ft_render.o: ft_render.cpp ft_render.h
	$(CC) $(CFLAGS) -c ft_render.cpp -o ft_render.o

ft_input.o: ft_input.cpp ft_input.h ft_data.h
	$(CC) $(CFLAGS) -c ft_input.cpp -o ft_input.o

ft_data.o: ft_data.cpp ft_data.h
	$(CC) $(CFLAGS) -c ft_data.cpp -o ft_data.o

ft_algorithm.o: ft_algorithm.cpp ft_algorithm.cpp
	$(CC) $(CFLAGS) -c ft_algorithm.cpp -o ft_algorithm.o
	
clean:
	rm -rf *.o fountain
