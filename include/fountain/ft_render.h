#ifndef _FT_RENDER_H_
#define _FT_RENDER_H_

#include <fountain/ft_data.h>
#include <map>

#define FT_CAMERA_NEAR 100.0f
#define FT_CAMERA_FAR 2000.0f

#define FT_PLANE 1
#define FT_PERSPECTIVE 2

#define FT_FORMAT_MAX 16
#define FT_RGB 1
#define FT_RGBA 2
#define FT_BGR 3
#define FT_BGRA 4
#define FT_GRAY 5
#define FT_GRAY_ALPHA 6

typedef struct {
	int id;
	int w, h;
} texInfo;

namespace ftRender {

bool init();
void close();
bool isAlive();

void frameBegin();
void frameEnd();

void clearColorDepthBuffer();

void transformBegin();
void transformBegin(float x, float y, float degree = 0.0f, float scale = 1.0f, ftColor c = FT_White);
void transformBegin(ftVec2 pos, float degree = 0.0f, float scale = 1.0f, ftColor c = FT_White);
void transformBegin(ftSprite *sprite);
void transformEnd();
void ftTranslate(float x, float y, float z = 0.0f);
void ftTranslate(ftVec2 xy, float z = 0.0f);
void ftRotate(float xAngle, float yAngle, float zAngle);
void ftScale(float scale);
void ftTransparency(float tp);
void ftColor4f(float r, float g, float b, float a);
void ftColor3f(float r, float g, float b);
void useColor(ftColor c);
ftColor getGlobalColor();

void setLineWidth(float w);

void openLineSmooth();
void openPointSmooth();
void openPolygonSmooth();
void setClearColor(ftColor c);

void drawLine(float x1, float y1, float x2, float y2);
void drawLine(const ftVec2 & p1, const ftVec2 & p2);
void drawLineArrow(const ftVec2 & p1, const ftVec2 & p2, float arrorSize = 10.0f);
void drawQuad(float w, float h);
void drawQuadEdge(float w, float h);
void drawRect(ftRect & rct, float angle = 0.0f);
void drawCircle(float radius);
void drawCircleEdge(float radius);
void drawShape(ftShape & shape, float angle = 0.0f);
void drawShapeEdge(ftShape & shape, float angle = 0.0f);

//TODO: implement this function drawBitmap
void drawBitmap(unsigned char *bits, int width, int height, int dataType = FT_RGBA, int x = 0, int y = 0);
int getPicture(unsigned char *bits, int width, int height, int dataType);
int getPicture(const char *filename, bool cubeMap = false);
ftVec2 getPicSize(int picID);
void drawPic(int picID);
void drawAlphaPic(int picID);

void deletePicture(int picID);
void deleteAllPictures();

void useBasicShader();
void useFFP();

class SubImage
{
private:
	int picID;
	ftVec2 size;
	float texCoor[8];
public:
	SubImage();
	SubImage(int picID);
	SubImage(int picID, ftRect & rect);
	SubImage(const char *picName, ftRect & rect);
	SubImage(SubImage image, ftRect & rect);
	void setPicID(int id);
	int getPicID();
	const ftVec2 & getSize();
	void setSize(const ftVec2 size);
	const float * getTexCoor();
};

SubImage getImage(int picID);
SubImage getImage(const char *filename);
void drawImage(SubImage & im);
void drawImage(SubImage & im, float x, float y, float degree = 0.0f, float scale = 1.0f, ftColor c = FT_White);

class SubImagePool
{
private:
	int picID;
	int picN;
	std::map<int, int> nameHash2SubImageIndex;
	std::vector<SubImage> subImageVec;
	std::map<int, int> getMapFromSip(int pid, const char * sipName);
public:
	SubImagePool();
	SubImagePool(const char *picName, const char *sipName);
	const SubImage & getImage(const char *imageName);
	const SubImage & getImageFromIndex(int index);
	int getImageNumber();
};

class Camera
{
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
	Camera(float x = 0.0f, float y = 0.0f, float z = 1000.0f);
	void update();
	void setPosition(float x, float y);
	void setPosition(ftVec2 vec);
	void setPosition(float x, float y, float z);
	ftVec2 getPosition();
	void move(const ftVec2 & v);
	void move(float x, float y);
	void setViewport(int w, int h, int x = 0, int y = 0);
	void setViewport(ftRect vp);
	void setScale(float scale);
	void setAngle(float x, float y, float z);
	void setProjectionType(int type);
	const ftVec2 mouseToWorld(const ftVec2 & mPos);
	const ftVec2 worldToScreen(const ftVec2 & wCoord);
	ftRect getCameraRect();
};

Camera* getCurrentCamera();

class ShaderProgram
{
private:
	ftFile vsFile, fsFile;
	unsigned vs, fs;
	unsigned program;
public:
	ShaderProgram();
	ShaderProgram(const char *vs, const char *vf);
	~ShaderProgram();
	void load(const char *vs, const char *vf);
	bool init();
	bool init(const char *vss, const char *fss);
	bool reload();
	void use();
	virtual void update();
	void setUniform(const char *varName, float value);
	void setUniform(const char *varName, const ftVec2 & value);
	void setTexture(const char *texName, int picID, int texID = 0);
	void free();
};

class Bitmap
{
public:
	unsigned char *bits;
	int width;
	int height;
	int type;
	Bitmap();
	void free();
	int getPicture();
	unsigned char getGray(int x, int y);
	void setGray(int x, int y, unsigned char c);
	void newImage(int w, int h, int type);
};

Bitmap getBitmapFromScreen(int x, int y, int w, int h);

}
#endif
