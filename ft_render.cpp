#include "ft_render.h"
#include <GL/gl.h>

void ftRender::init()
{
}

using namespace ftRender;

Camera::Camera(float x, float y, float z)
{
	Camera::setPosition(x, y, z);
	Camera::setWinSize(1.0f, 1.0f);
	Camera::setScale(1.0f);
	Camera::setAngle(0.0f, 0.0f, 0.0f);
}

void Camera::setPosition(float x, float y, float z)
{
	Camera::x = x;
	Camera::y = y;
	if (z < FT_CAMERA_NEAR) z = FT_CAMERA_NEAR;
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
	Camera::ratio = z / FT_CAMERA_NEAR;
	Camera::nearW2 = Camera::winW / Camera::ratio / 2.0f;
	Camera::nearH2 = Camera::winH / Camera::ratio / 2.0f;
}

void Camera::setScale(float scale)
{
	Camera::scale = scale;
}

void Camera::update()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-Camera::nearW2, Camera::nearW2, -Camera::nearH2, Camera::nearH2, FT_CAMERA_NEAR, FT_CAMERA_FAR);
	glRotatef(Camera::xAngle, 1.0f, 0.0f, 0.0f);
	glRotatef(Camera::yAngle, 0.0f, 1.0f, 0.0f);
	glRotatef(Camera::zAngle, 0.0f, 0.0f, 1.0f);
	glTranslatef(-Camera::x, -Camera::y, -Camera::z);
	glScalef(Camera::scale, Camera::scale, Camera::scale);
	glMatrixMode(GL_MODELVIEW);
}
