#ifndef _FT_3DMODEL_
#define _FT_3DMODEL_

namespace ft3DModel {

bool init();
void close();

class ObjModel
{
private:
	int vecN;
	int indexN;
	float (*vtx)[3];
public:
	ObjModel();
	explicit ObjModel(const char *fileName);
	~ObjModel();
	void loadObj(const char *fileName);
	void render();
};

}
#endif
