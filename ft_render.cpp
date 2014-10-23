#include "ft_render.h"
#include "ft_data.h"
#include "ft_algorithm.h"
#include <GL/gl.h>
#include <GL/glext.h>
#include <FreeImage.h>
#include <map>

static std::map < int, int >Hash2PicID;
static std::map < int, texInfo > PicID2TexInfo;
static int curPicID = 1;

void ftRender::init()
{

}

void ftRender::transformBegin()
{
	glPushMatrix();
}

void ftRender::transformEnd()
{
	glPopMatrix();
}

void ftRender::ftTranslate(float x, float y, float z)
{
	glTranslatef(x, y, z);
}

void ftRender::ftRotate(float xAngle, float yAngle, float zAngle)
{
	glRotatef(xAngle, 1.0f, 0.0f, 0.0f);
	glRotatef(yAngle, 0.0f, 1.0f, 0.0f);
	glRotatef(zAngle, 0.0f, 0.0f, 1.0f);
}

void ftRender::ftScale(float scale)
{
	glScalef(scale, scale, scale);
}

void ftRender::openLineSmooth()
{
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

texInfo loadTexture(const char *filename)
{
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	FIBITMAP *dib;
	BYTE *bits;
	unsigned int width, height;
	GLuint gl_texID;
	texInfo tex;
	tex.id = -1;
	tex.w = 0;
	tex.h = 0;
	fif = FreeImage_GetFileType(filename, 0);
	if (fif == FIF_UNKNOWN)
		fif = FreeImage_GetFIFFromFilename(filename);
	if (FreeImage_FIFSupportsReading(fif))
		dib = FreeImage_Load(fif, filename, 0);
	else
		return tex;
	bits = FreeImage_GetBits(dib);
	width = FreeImage_GetWidth(dib);
	height = FreeImage_GetHeight(dib);
	glGenTextures(1, &gl_texID);
	glBindTexture(GL_TEXTURE_2D, gl_texID);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	if (fif == FIF_PNG)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
			     GL_BGRA, GL_UNSIGNED_BYTE, bits);
	else
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR,
			     GL_UNSIGNED_BYTE, bits);
	FreeImage_Unload(dib);
	tex.id = gl_texID;
	tex.w = width;
	tex.h = height;
	return tex;
}

int ftRender::getPicture(const char *filename)
{
	texInfo texIf;
	int hash = ftAlgorithm::bkdrHash(filename);
	if (Hash2PicID[hash] == 0) {
		texIf = loadTexture(filename);
		PicID2TexInfo[curPicID] = texIf;
		Hash2PicID[hash] = curPicID;
		return curPicID++;
	} else
		return Hash2PicID[hash];
}

void ftRender::drawPic(int picID)
{
	texInfo tex = PicID2TexInfo[picID];
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glBindTexture(GL_TEXTURE_2D, tex.id);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex2f(-tex.w / 2.0f, -tex.h / 2.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex2f(tex.w / 2.0f, -tex.h / 2.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex2f(tex.w / 2.0f, tex.h / 2.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex2f(-tex.w / 2.0f, tex.h / 2.0f);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void ftRender::drawAlphaPic(int picID)
{
	texInfo tex = PicID2TexInfo[picID];
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, tex.id);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex2f(-tex.w / 2.0f, -tex.h / 2.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex2f(tex.w / 2.0f, -tex.h / 2.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex2f(tex.w / 2.0f, tex.h / 2.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex2f(-tex.w / 2.0f, tex.h / 2.0f);
	glEnd();
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
}

using namespace ftRender;

Camera::Camera(float x, float y, float z)
{
	Camera::setPosition(x, y, z);
	Camera::setWinSize(fountain::mainWin.w, fountain::mainWin.h);
	Camera::setScale(1.0f);
	Camera::setAngle(0.0f, 0.0f, 0.0f);
	Camera::setProjectionType(FT_PLANE);
}

void Camera::setPosition(float x, float y)
{
	Camera::x = x;
	Camera::y = y;
}

void Camera::setPosition(float x, float y, float z)
{
	Camera::x = x;
	Camera::y = y;
	if (z < FT_CAMERA_NEAR)
		z = FT_CAMERA_NEAR;
	Camera::z = z;
}

void Camera::setAngle(float x, float y, float z)
{
	Camera::xAngle = x;
	Camera::yAngle = y;
	Camera::zAngle = z;
}

void Camera::setWinSize(int w, int h)
{
	Camera::winW = (float)w;
	Camera::winH = (float)h;
	Camera::W2 = Camera::winW / 2.0f;
	Camera::H2 = Camera::winH / 2.0f;
	Camera::ratio = Camera::z / FT_CAMERA_NEAR;
	Camera::nearW2 = Camera::winW / Camera::ratio / 2.0f;
	Camera::nearH2 = Camera::winH / Camera::ratio / 2.0f;
}

void Camera::setScale(float scale)
{
	if (scale < 0)
		scale = 0.01;
	Camera::scale = scale;
}

void Camera::setProjectionType(int type)
{
	Camera::projectionType = type;
}

void Camera::update()
{
	glViewport(0, 0, Camera::winW, Camera::winH);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (Camera::projectionType == FT_PLANE) {
		glOrtho(-Camera::W2, Camera::W2, -Camera::H2, Camera::H2,
			-99999, 99999);
	} else if (Camera::projectionType == FT_PERSPECTIVE) {
		glFrustum(-Camera::nearW2 / Camera::scale,
			  Camera::nearW2 / Camera::scale,
			  -Camera::nearH2 / Camera::scale,
			  Camera::nearH2 / Camera::scale, FT_CAMERA_NEAR,
			  FT_CAMERA_FAR);
	}
	glRotatef(Camera::xAngle, 1.0f, 0.0f, 0.0f);
	glRotatef(Camera::yAngle, 0.0f, 1.0f, 0.0f);
	glRotatef(Camera::zAngle, 0.0f, 0.0f, 1.0f);
	glTranslatef(-Camera::x, -Camera::y, -Camera::z);
	glMatrixMode(GL_MODELVIEW);
}
