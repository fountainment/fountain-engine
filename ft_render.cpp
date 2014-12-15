#include <fountain/ft_render.h>
#include <fountain/ft_data.h>
#include <fountain/ft_algorithm.h>
#include <GL/glew.h>
#include <FreeImage.h>
#include <map>
#include <cmath>
#include <cstdio>

using ftRender::SubImage;
using ftRender::SubImagePool;
using ftRender::Camera;

static std::map<int, int> Hash2PicID;
static std::map<int, texInfo> PicID2TexInfo;
static int curPicID = 1;

static GLfloat circle8[8][2];
static GLfloat circle32[32][2];
static GLfloat circle128[128][2];

//static GLuint VertexArrayID;

static float globalR, globalG, globalB, globalA;

//GLSL exp
static GLuint vs ,fs;
GLint compiled, linked;
GLint timeLoc;
GLfloat timeX = 0.0f;
ftFile vsf("resources/vs.vert");
ftFile fsf("resources/fs.frag");
const GLchar *vss = vsf.getStr();
const GLchar *fss = fsf.getStr();
GLuint program;

void initCircleData(GLfloat (*v)[2], int n)
{
	float d = 3.14159f * 2.0f / n;
	for (int i = 0; i < n; i++) {
		v[i][0] = std::sin(d * i);
		v[i][1] = std::cos(d * i);
	}
}

bool GLinit()
{
	//TODO: check the OpenGL init state
	glewInit();
	std::printf("OpenGL Version: %s\n", glGetString(GL_VERSION));
	std::printf("GLSL Version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	return true;
}

void ftRender::init()
{
	GLinit();

	initCircleData(circle8, 8);
	initCircleData(circle32, 32);
	initCircleData(circle128, 128);

	globalR = 1.0f;
	globalG = 1.0f;
	globalB = 1.0f;
	globalA = 1.0f;

//	glGenVertexArrays(1, &VertexArrayID);
//	glBindVertexArray(VertexArrayID);

	//TODO: add class ShaderManager to deal with shader
	//GLSL exp
	vs = glCreateShader(GL_VERTEX_SHADER);
	fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(vs, 1, &vss, NULL);
	glShaderSource(fs, 1, &fss, NULL);
	glCompileShader(vs);
	glGetShaderiv(vs, GL_COMPILE_STATUS, &compiled);
	if (!compiled) std::printf("vertex shader compile failed!!!\n");
	glCompileShader(fs);
	glGetShaderiv(fs, GL_COMPILE_STATUS, &compiled);
	if (!compiled) std::printf("fragment shader compile failed!!!\n");
	program = glCreateProgram();
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	timeLoc = glGetUniformLocation(program, "time");
	glUniform1f(timeLoc, timeX);
	glUseProgram(program);
}

void glDrawVectorVec2(const float * v, int n, GLuint glType)
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, v);
	glDrawArrays(glType, 0, n);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void ftRender::transformBegin()
{
	timeX += 0.01f;
	if (timeX > 314.0f) timeX -= 314.0f;
	glUniform1f(timeLoc, timeX);
	glPushMatrix();
}

void ftRender::transformEnd()
{
	glPopMatrix();
	ftColor4f(1.0f, 1.0, 1.0f, 1.0f);
	timeX += 0.01f;
	if (timeX > 314.0f) timeX -= 314.0f;
	glUniform1f(timeLoc, timeX);
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
	glColor4f(globalR, globalG, globalB, tp);
}

void ftRender::ftColor4f(float r, float g, float b, float a)
{
	globalR = r;
	globalG = g;
	globalB = b;
	globalA = a;
	glColor4f(r, g, b, a);
}

void ftRender::ftColor3f(float r, float g, float b)
{
	globalR = r;
	globalG = g;
	globalB = b;
	glColor3f(r, g, b);
}

void ftRender::useColor(ftColor c)
{
	ftRender::ftColor4f(c.getR(), c.getG(), c.getB(), c.getAlpha());
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
	GLfloat vtx1[] = {x1, y1, x2, y2};
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, vtx1);
	glDrawArrays(GL_LINES, 0, 2);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void ftRender::drawLine(ftVec2 p1, ftVec2 p2)
{
	ftRender::drawLine(p1.x, p1.y, p2.x, p2.y);
}

void ftRender::drawQuad(float w, float h)
{
	float w2 = w / 2.0f;
	float h2 = h / 2.0f;
	GLfloat vtx1[] = {-w2, -h2, -w2, h2, w2, h2, w2, -h2};
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, vtx1);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glDisableClientState(GL_VERTEX_ARRAY);
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
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, circle32);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 32);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void ftRender::drawShape(ftShape & shape, float angle)
{
	int type = shape.getType();
	const float *v = shape.getData();
	int n = shape.getN();
	ftRender::ftRotate(0.0f, 0.0f, angle);
	switch (type)
	{
	case FT_Circle:
		ftRender::ftScale(shape.getR());
		ftRender::drawCircle();
		break;

	case FT_Polygon:
		glDrawVectorVec2(v, n, GL_TRIANGLE_FAN);
		break;

	case FT_Line:
		glDrawVectorVec2(v, n, GL_LINE_STRIP);
		break;

	case FT_Rect:
		ftRender::drawQuad(v[0], v[1]);
		break;
	}
}

ftVec2 ftRender::getPicSize(int picID)
{
	texInfo tex = PicID2TexInfo[picID];
	return ftVec2(tex.w, tex.h);
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

//class ftRender::SubImage
SubImage::SubImage()
{
}

SubImage::SubImage(int picID, ftRect rect)
{
	this->picID = picID;
	size = rect.getSize();
	ftRect texRect = rect;
	ftVec2 pSize = ftRender::getPicSize(picID);
	texRect.inflate(1.0f / pSize.x, 1.0f / pSize.y);
	texCoor[0] = texRect.getLB();
	texCoor[1] = texRect.getRB();
	texCoor[2] = texRect.getRT();
	texCoor[3] = texRect.getLT();
}

SubImage::SubImage(const char * picName, ftRect rect)
{
	picID = ftRender::getPicture(picName);
	size = rect.getSize();
	ftRect texRect = rect;
	ftVec2 pSize = ftRender::getPicSize(picID);
	texRect.inflate(1.0f / pSize.x, 1.0f / pSize.y);
	texCoor[0] = texRect.getLB();
	texCoor[1] = texRect.getRB();
	texCoor[2] = texRect.getRT();
	texCoor[3] = texRect.getLT();
}

//class ftRender::SubImagePool
std::map<int, SubImage> SubImagePool::getMapFromSip(int pid, const char * sipName)
{
	int x, y;
	int picN;
	char name[100];
	int rx, ry, rw, rh;
	int tmp;
	std::map<int, SubImage> ans;
	std::FILE *sipF = std::fopen(sipName, "r");
	if (sipF == NULL) return ans;
	tmp = std::fscanf(sipF, "%d %d", &x, &y);
	tmp = std::fscanf(sipF, "%d", &picN);
	for (int i = 0; i < picN; i++) {
		tmp = std::fscanf(sipF, "%s %d %d %d %d", name, &rw, &rh, &rx, &ry);
		if (tmp == EOF) break;
		ftRect r(rx, y - ry - rh, rw, rh);
		int hash = ftAlgorithm::bkdrHash(name);
		ans[hash] = SubImage(pid, r);
	}
	std::fclose(sipF);

	return ans;
}

SubImagePool::SubImagePool(const char * picName, const char * sipName)
{
	picID = ftRender::getPicture(picName);
	nameHash2SubImage = getMapFromSip(picID, sipName);
}

const SubImage & SubImagePool::getImage(const char * imageName)
{
	int hash = ftAlgorithm::bkdrHash(imageName);
	return nameHash2SubImage[hash];
}

//test
void ftRender::drawImage(SubImage im)
{
	texInfo tex = PicID2TexInfo[im.picID];
	int w2 = im.size.x / 2.0f;
	int h2 = im.size.y / 2.0f;
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, tex.id);
	glBegin(GL_TRIANGLE_FAN);
	glTexCoord2f(im.texCoor[0].x, im.texCoor[0].y);
	glVertex2f(-w2, -h2);
	glTexCoord2f(im.texCoor[1].x, im.texCoor[1].y);
	glVertex2f(w2, -h2);
	glTexCoord2f(im.texCoor[2].x, im.texCoor[2].y);
	glVertex2f(w2, h2);
	glTexCoord2f(im.texCoor[3].x, im.texCoor[3].y);
	glVertex2f(-w2, h2);
	glEnd();
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
}

//class ftRender::Camera
Camera::Camera(float x, float y, float z)
{
	setPosition(x, y, z);
	setViewport(fountain::mainWin.w, fountain::mainWin.h);
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

void Camera::setViewport(int w, int h, int x, int y)
{
	viewport = ftRect(x, y, w, h);
	W2 = viewport.getW() / 2.0f;
	H2 = viewport.getH() / 2.0f;
	ratio = z / FT_CAMERA_NEAR;
	nearW2 = viewport.getW() / ratio / 2.0f;
	nearH2 = viewport.getH() / ratio / 2.0f;
}

void Camera::setViewport(ftRect vp)
{
	this->setViewport(vp.getW(), vp.getH(), vp.getX(), vp.getY());
}

void Camera::setScale(float scale)
{
	if (scale == 0)
		scale = 0.001;
	this->scale = scale;
}

void Camera::setProjectionType(int type)
{
	projectionType = type;
}

void Camera::update()
{
	glViewport(viewport.getX(), viewport.getY(), viewport.getW(), viewport.getH());
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
