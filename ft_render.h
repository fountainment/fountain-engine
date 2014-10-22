#ifndef _FT_RENDER_H_
#define _FT_RENDER_H_

#define FT_CAMERA_NEAR 100.0f
#define FT_CAMERA_FAR 1000.0f

#define FT_PLANE 1
#define FT_PERSPECTIVE 2

typedef struct {
	int id;
	int w, h;
}texInfo;

namespace ftRender {
	void init();

	void transformBegin();

	void transformEnd();

	void openLineSmooth();

	int getPicture(const char *filename);

	void drawPic(int picID);

	void drawAlphaPic(int picID);

	class Camera {
private:
		float x, y, z;
		float winW, winH;
		float W2, H2;
		float nearW2, nearH2;
		float scale;
		float xAngle, yAngle, zAngle;
		float ratio;
		int projectionType;
public:
		 Camera(float x = 0.0f, float y = 0.0f, float z = 0.0f);
		void update();
		void setPosition(float x, float y);
		void setPosition(float x, float y, float z);
		void setWinSize(int w, int h);
		void setScale(float scale);
		void setAngle(float x, float y, float z);
		void setProjectionType(int type);
	};
}
#endif
