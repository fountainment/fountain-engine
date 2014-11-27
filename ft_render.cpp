#include <fountain/ft_render.h>
#include <fountain/ft_data.h>
#include <fountain/ft_algorithm.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <FreeImage.h>
#include <map>
#include <cmath>

using ftRender::Camera;

static std::map<int, int> Hash2PicID;
static std::map<int, texInfo> PicID2TexInfo;
static int curPicID = 1;
static ftVec2 circle8[8];
static ftVec2 circle32[32];
static ftVec2 circle128[128];

void initCircleData(ftVec2 *v, int n)
{
	float d = 3.14159f * 2.0f / n;
	for (int i = 0; i < n; i++) {
		v[i].x = std::sin(d * i);
		v[i].y = std::cos(d * i);
	}
}

void glDrawArrayFloat2(float (*a)[2], int n, int glType)
{
	glBegin(glType);
	for (int i = 0; i < n; i++)
		glVertex2fv(a[i]);
	glEnd();
}

void glDrawVectorVec2(std::vector<ftVec2> & v, int glType)
{
	glBegin(glType);
	for (unsigned int i = 0; i < v.size(); i++) {
		glVertex2f(v[i].x, v[i].y);
	}
	glEnd();
}

void ftRender::init()
{
	initCircleData(circle8, 8);
	initCircleData(circle32, 32);
	initCircleData(circle128, 128);
}

void ftRender::transformBegin()
{
	glPushMatrix();
}

void ftRender::transformEnd()
{
	glPopMatrix();
	glColor4f(1.0f, 1.0, 1.0f, 1.0f);
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

void ftRender::ftTransparency(float tp)
{
    glColor4f(1.0, 1.0, 1.0, tp);
}

void ftRender::ftColor4f(float r, float g, float b, float a)
{
	glColor4f(r, g, b, a);
}

void ftRender::openLineSmooth()
{
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ftRender::openPointSmooth()
{
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ftRender::setClearColor(int r, int g, int b)
{
	glClearColor(r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);
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

void ftRender::drawLine(float x1, float y1, float x2, float y2)
{
	glBegin(GL_LINES);
	glVertex2f(x1, y1);
	glVertex2f(x2, y2);
	glEnd();
}

void ftRender::drawLine(ftVec2 p1, ftVec2 p2)
{
	ftRender::drawLine(p1.x, p1.y, p2.x, p2.y);
}

void ftRender::drawQuad(float w, float h)
{
	float w2 = w / 2.0f;
	float h2 = h / 2.0f;
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBegin(GL_QUADS);
	glVertex2f(-w2, -h2);
	glVertex2f(-w2, h2);
	glVertex2f(w2, h2);
	glVertex2f(w2, -h2);
	glEnd();
	glDisable(GL_BLEND);
}

void ftRender::drawRect(ftRect rct, float angle)
{
	ftVec2 rPos = rct.getCenter();
	ftVec2 rSize = rct.getSize();
	ftRender::transformBegin();
	ftRender::ftTranslate(rPos.x, rPos.y);
	ftRender::ftRotate(0.0f, 0.0f, angle);
	ftRender::drawQuad(rSize.x, rSize.y);
	ftRender::transformEnd();
}

void ftRender::drawCircle()
{
	glBegin(GL_TRIANGLE_FAN);
	for (int i = 0; i < 32; i++) {
		glVertex2f(circle32[i].x, circle32[i].y);
	}
	glEnd();
}

void ftRender::drawShape(ftShape & shape, float angle)
{
	int type = shape.getType();
	std::vector<ftVec2> v = shape.getData();
	ftRender::ftRotate(0.0f, 0.0f, angle);
	switch (type)
	{
	case FT_Circle:
		ftRender::ftScale(shape.getR());
		ftRender::drawCircle();
		break;

	case FT_Polygon:
		glDrawVectorVec2(v, GL_TRIANGLE_FAN);
		break;

	case FT_Line:
		glDrawVectorVec2(v, GL_LINE_STRIP);
		break;

	case FT_Rect:
		ftRender::drawQuad(v[0].x, v[0].y);
		break;
	}
}

void ftRender::drawPic(int picID)
{
	texInfo tex = PicID2TexInfo[picID];
	int w2 = tex.w / 2.0f;
	int h2 = tex.h / 2.0f;
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glBindTexture(GL_TEXTURE_2D, tex.id);
	glBegin(GL_TRIANGLE_FAN);
	glTexCoord2f(0.0f, 0.0f);
	glVertex2f(-w2, -h2);
	glTexCoord2f(1.0f, 0.0f);
	glVertex2f(w2, -h2);
	glTexCoord2f(1.0f, 1.0f);
	glVertex2f(w2, h2);
	glTexCoord2f(0.0f, 1.0f);
	glVertex2f(-w2, h2);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void ftRender::drawAlphaPic(int picID)
{
	texInfo tex = PicID2TexInfo[picID];
	int w2 = tex.w / 2.0f;
	int h2 = tex.h / 2.0f;
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, tex.id);
	glBegin(GL_TRIANGLE_FAN);
	glTexCoord2f(0.0f, 0.0f);
	glVertex2f(-w2, -h2);
	glTexCoord2f(1.0f, 0.0f);
	glVertex2f(w2, -h2);
	glTexCoord2f(1.0f, 1.0f);
	glVertex2f(w2, h2);
	glTexCoord2f(0.0f, 1.0f);
	glVertex2f(-w2, h2);
	glEnd();
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
}

Camera::Camera(float x, float y, float z)
{
	setPosition(x, y, z);
	setWinSize(fountain::mainWin.w, fountain::mainWin.h);
	setScale(1.0f);
	setAngle(0.0f, 0.0f, 0.0f);
	setProjectionType(FT_PLANE);
}

void Camera::setPosition(float x, float y)
{
	this->x = x;
	this->y = y;
}

void Camera::setPosition(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	if (z < FT_CAMERA_NEAR)
		z = FT_CAMERA_NEAR;
	this->z = z;
}

void Camera::move(float x, float y)
{
	this->x += x;
	this->y += y;
}

void Camera::setAngle(float x, float y, float z)
{
	this->xAngle = x;
	this->yAngle = y;
	this->zAngle = z;
}

void Camera::setWinSize(int w, int h)
{
	winW = (float)w;
	winH = (float)h;
	W2 = winW / 2.0f;
	H2 = winH / 2.0f;
	ratio = z / FT_CAMERA_NEAR;
	nearW2 = winW / ratio / 2.0f;
	nearH2 = winH / ratio / 2.0f;
}

void Camera::setScale(float scale)
{
	if (scale < 0)
		this->scale = 0.01;
	this->scale = scale;
}

void Camera::setProjectionType(int type)
{
	projectionType = type;
}

void Camera::update()
{
	glViewport(0, 0, winW, winH);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (projectionType == FT_PLANE) {
		glOrtho(-W2 / scale, W2 / scale, -H2 / scale, H2 / scale,
		        -99999, 99999);
	} else if (projectionType == FT_PERSPECTIVE) {
		glFrustum(-nearW2 / scale, nearW2 / scale, -nearH2 / scale,
		          nearH2 / scale, FT_CAMERA_NEAR, FT_CAMERA_FAR);
	}
	glRotatef(xAngle, 1.0f, 0.0f, 0.0f);
	glRotatef(yAngle, 0.0f, 1.0f, 0.0f);
	glRotatef(zAngle, 0.0f, 0.0f, 1.0f);
	glTranslatef(-x, -y, -z);
	glMatrixMode(GL_MODELVIEW);
}

ftVec2 Camera::mouseToWorld(ftVec2 mPos)
{
	float l, b, w2, h2;
	w2 = W2 / scale;
	h2 = H2 / scale;
	l = x - w2;
	b = y - h2;
	ftVec2 ans;
	ans.x = mPos.x / scale + l;
	ans.y = mPos.y / scale + b;
	return ans;
}
