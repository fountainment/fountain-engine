#ifndef _FT_RENDER_H_
#define _FT_RENDER_H_

#include <fountain/ft_data.h>
#include <map>

#define FT_CAMERA_NEAR 100.0f
#define FT_CAMERA_FAR 1000.0f

#define FT_PLANE 1
#define FT_PERSPECTIVE 2

typedef struct {
	int id;
	int w, h;
} texInfo;

namespace ftRender {
void init();

void transformBegin();
void transformEnd();
void ftTranslate(float x, float y, float z = 0.0f);
void ftRotate(float xAngle, float yAngle, float zAngle);
void ftScale(float scale);
void ftTransparency(float tp);
void ftColor4f(float r, float g, float b, float a);
void ftColor3f(float r, float g, float b);
void useColor(ftColor c);

void openLineSmooth();
void openPointSmooth();
void setClearColor(int r, int g, int b);

void drawLine(float x1, float y1, float x2, float y2);
void drawLine(ftVec2 p1, ftVec2 p2);
void drawQuad(float w, float h);
void drawRect(ftRect rct, float angle = 0.0f);
void drawCircle();
void drawShape(ftShape & shape, float angle = 0.0f);

int getPicture(const char *filename);
ftVec2 getPicSize(int picID);
void drawPic(int picID);
void drawAlphaPic(int picID);

void useShader();
void useFFP();

class SubImage {
private:
public:
	int picID;
	ftVec2 size;
	ftVec2 texCoor[4];
//public:
	SubImage();
	SubImage(int picID, ftRect rect);
	SubImage(const char * picName, ftRect rect);
};

void drawImage(SubImage im);

class SubImagePool {
private:
	int picID;
	std::map<int, SubImage> nameHash2SubImage;
	static std::map<int, SubImage> getMapFromSip(int pid, const char * sipName);
public:
	SubImagePool(const char * picName, const char * sipName);
	const SubImage & getImage(const char * imageName);
};

class Camera {
private:
	float x, y, z;
	float W2, H2;
	float nearW2, nearH2;
	float scale;
	float xAngle, yAngle, zAngle;
	float ratio;
	ftRect viewport;
	int projectionType;
public:
	Camera(float x = 0.0f, float y = 0.0f, float z = 0.0f);
	void update();
	void setPosition(float x, float y);
	void setPosition(float x, float y, float z);
	void move(float x, float y);
	void setViewport(int w, int h, int x = 0, int y = 0);
	void setViewport(ftRect vp);
	void setScale(float scale);
	void setAngle(float x, float y, float z);
	void setProjectionType(int type);
	ftVec2 mouseToWorld(ftVec2 mPos);
};

class ShaderProgram {
private:
	ftFile vsFile, fsFile;
	unsigned vs, fs;
	unsigned program;
public:
	ShaderProgram(const char *vs, const char *vf);
	~ShaderProgram();
	bool init();
	bool reload();
	void use();
	void setUniform(const char *varName, float value);
	void setUniform(const char *varName, ftVec2 value);
	void free();
};

}
#endif
