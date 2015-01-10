CPPS = ft_time.cpp ft_3dmodel.cpp ft_render.cpp ft_input.cpp ft_data.cpp ft_algorithm.cpp ft_physics.cpp ft_scene.cpp ft_type.cpp ft_audio.cpp ft_ui.cpp
OBJS = ft_time.o ft_3dmodel.o ft_render.o ft_input.o ft_data.o ft_algorithm.o ft_physics.o ft_scene.o ft_type.o ft_audio.o ft_ui.o
CC = g++
CFLAGS = -Wall -fexceptions -Iinclude -I. -c
FREETYPE = -I/usr/include/freetype2
FT_H = include/fountain

libfountain.a: $(OBJS)
	rm -f libfountain.a
	ar -r -s libfountain.a $(OBJS)

ft_time.o: ft_time.cpp $(FT_H)/ft_time.h
	$(CC) -O2 $(CFLAGS) ft_time.cpp

ft_3dmodel.o: ft_3dmodel.cpp $(FT_H)/ft_3dmodel.h $(FT_H)/ft_data.h
	$(CC) -O2 $(CFLAGS) ft_3dmodel.cpp

ft_render.o: ft_render.cpp $(FT_H)/ft_render.h $(FT_H)/ft_algorithm.h
	$(CC) -O2 $(CFLAGS) ft_render.cpp

ft_input.o: ft_input.cpp $(FT_H)/ft_input.h $(FT_H)/ft_data.h
	$(CC) -O2 $(CFLAGS) ft_input.cpp

ft_data.o: ft_data.cpp $(FT_H)/ft_data.h $(FT_H)/ft_render.h
	$(CC) -O2 $(CFLAGS) ft_data.cpp

ft_algorithm.o: ft_algorithm.cpp $(FT_H)/ft_algorithm.h
	$(CC) -O2 $(CFLAGS) ft_algorithm.cpp

ft_physics.o: ft_physics.cpp $(FT_H)/ft_physics.h
	$(CC) -O1 $(CFLAGS) ft_physics.cpp

ft_scene.o: ft_scene.cpp $(FT_H)/ft_scene.h $(FT_H)/fountaindef.h
	$(CC) -O2 $(CFLAGS) ft_scene.cpp

ft_type.o: ft_type.cpp $(FT_H)/ft_type.h $(FT_H)/ft_render.h $(FT_H)/ft_algorithm.h
	$(CC) -O2 $(CFLAGS) $(FREETYPE) ft_type.cpp

ft_audio.o: ft_audio.cpp #(FT_H)/ft_audio.h
	$(CC) -O2 $(CFLAGS) ft_audio.cpp

ft_ui.o: ft_ui.cpp #(FT_H)/ft_ui.h
	$(CC) -O2 $(CFLAGS) $(FREETYPE) ft_ui.cpp

clean:
	rm -rf *.o libfountain.a
