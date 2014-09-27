#ifndef FT_DATA_H_
#define FT_DATA_H_

#include <string>

typedef struct {
	int w, h;
	int w4, h4;
	std::string title;
}winState;

typedef struct {
	float x, y;
}ftVec2;

typedef struct {
	float xyz[3];

	float getX()
	{
		return xyz[0];
	}

	float getY()
	{
		return xyz[1];
	}

	float getZ()
	{
		return xyz[2];
	}

	float* getV()
	{
		return xyz;
	}
} ftVec3;

namespace fountain
{
	extern winState mainWin;
}

#endif
