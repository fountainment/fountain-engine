#ifndef _FT_3DMODEL_
#define _FT_3DMODEL_

#include <GL/glew.h>

namespace ft3DModel {

bool init();
void close();

class ObjModel
{
private:
	int vecN;
	int indexN;
	GLuint vboV;
	GLuint vboN;
public:
	ObjModel();
	explicit ObjModel(const char *fileName);
	~ObjModel();
	void loadObj(const char *fileName, bool smooth = false);
	void render();
};

}
#endif
