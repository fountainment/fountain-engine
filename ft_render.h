#ifndef FT_RENDER_H_
#define FT_RENDER_H_

#define FT_CAMERA_NEAR 100.0f
#define FT_CAMERA_FAR 1000.0f

namespace ftRender
{
	void init();

	void transformBegin();

	void transformEnd();

	class Camera
	{
		private:
			float x, y, z;
			float winW, winH;
			float nearW2, nearH2;
			float scale;
			float xAngle, yAngle, zAngle;
			float ratio;
		public:
			Camera(float x = 0.0f, float y = 0.0f, float z = 0.0f);
			void update();
			void setPosition(float x, float y);
			void setPosition(float x, float y, float z);
			void setWinSize(int w, int h);
			void setScale(float scale);
			void setAngle(float x, float y, float z);
	};
}

#endif
