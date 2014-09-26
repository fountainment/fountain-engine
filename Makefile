OBJS = main.o ft_time.o ft_3dmodel.o
LIBS = -lGL -lX11
CC = g++
CFLAGS = -O
fountain: $(OBJS)
	$(CC) $(OBJS) -o fountain $(LIBS)

main.o: main.cpp fountain.h
	$(CC) $(CFLAGS) -c main.cpp -o main.o

ft_time.o: ft_time.cpp ft_time.h
	$(CC) $(CFLAGS) -c ft_time.cpp -o ft_time.o

ft_3dmodel.o: ft_3dmodel.cpp ft_3dmodel.h
	$(CC) $(CFLAGS) -c ft_3dmodel.cpp -o ft_3dmodel.o

clean:
	rm -rf *.o fountain
