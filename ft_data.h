#ifndef FT_DATA_H_
#define FT_DATA_H_

#include <string>

typedef struct {
	int w, h;
	float w4, h4;
	std::string title;
	std::string icon;
	bool isFullScreen;
	bool hideCursor;

	void setW(int wv)
	{
        w = wv;
        w4 = w / 4.0f;
	}

	void setH(int hv)
	{
        h = hv;
        h4 = h / 4.0f;
	}

}winState;

typedef struct {
	float x, y;

	float getX()
	{
		return x;
	}

	float getY()
	{
		return y;
	}

} ftVec2;

extern const ftVec2 operator-(const ftVec2 & a, const ftVec2 & b);

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
