CPPS = ft_time.cpp ft_3dmodel.cpp ft_render.cpp ft_input.cpp ft_data.cpp ft_algorithm.cpp ft_physics.cpp ft_scene.cpp ft_type.cpp ft_audio.cpp ft_ui.cpp ft_anime.cpp ft_debug.cpp ft_fountain.cpp
OBJS = ft_time.o ft_3dmodel.o ft_render.o ft_input.o ft_data.o ft_algorithm.o ft_physics.o ft_scene.o ft_type.o ft_audio.o ft_ui.o ft_fountain.o ft_anime.o ft_debug.o
CC = g++
CFLAGS = -Wall -g -std=c++11 -DDEBUG -fexceptions -Iinclude -I. -I/usr/include/freetype2 -c
FT_H = include/fountain

libfountain.a: $(OBJS)
	rm -f libfountain.a
	ar -r -s libfountain.a $(OBJS)

ft_time.o: ft_time.cpp $(FT_H)/ft_time.h
	$(CC) $(CFLAGS) ft_time.cpp

ft_3dmodel.o: ft_3dmodel.cpp $(FT_H)/ft_3dmodel.h $(FT_H)/ft_data.h
	$(CC) $(CFLAGS) ft_3dmodel.cpp

ft_render.o: ft_render.cpp $(FT_H)/ft_render.h $(FT_H)/ft_algorithm.h
	$(CC) $(CFLAGS) ft_render.cpp

ft_input.o: ft_input.cpp $(FT_H)/ft_input.h $(FT_H)/ft_data.h
	$(CC) $(CFLAGS) ft_input.cpp

ft_data.o: ft_data.cpp $(FT_H)/ft_data.h $(FT_H)/ft_render.h
	$(CC) $(CFLAGS) ft_data.cpp

ft_algorithm.o: ft_algorithm.cpp $(FT_H)/ft_algorithm.h
	$(CC) $(CFLAGS) ft_algorithm.cpp

ft_physics.o: ft_physics.cpp $(FT_H)/ft_physics.h
	$(CC) $(CFLAGS) ft_physics.cpp

ft_scene.o: ft_scene.cpp $(FT_H)/ft_scene.h $(FT_H)/fountaindef.h
	$(CC) $(CFLAGS) ft_scene.cpp

ft_type.o: ft_type.cpp $(FT_H)/ft_type.h $(FT_H)/ft_render.h $(FT_H)/ft_algorithm.h
	$(CC) $(CFLAGS) ft_type.cpp

ft_audio.o: ft_audio.cpp #(FT_H)/ft_audio.h
	$(CC) $(CFLAGS) ft_audio.cpp

ft_ui.o: ft_ui.cpp #(FT_H)/ft_ui.h
	$(CC) $(CFLAGS) ft_ui.cpp

ft_anime.o: ft_anime.cpp #(FT_H)/ft_anime.h
	$(CC) $(CFLAGS) ft_anime.cpp

ft_debug.o: ft_debug.cpp #(FT_H)/ft_debug.h
	$(CC) $(CFLAGS) ft_debug.cpp

ft_fountain.o: ft_fountain.cpp #(FT_H)/fountaindef.h
	$(CC) $(CFLAGS) ft_fountain.cpp

clean:
	rm -rf *.o libfountain.a
