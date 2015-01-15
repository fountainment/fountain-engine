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
	int neverRendered;
	int listIndex;
public:
	explicit ObjModel(const char *fileName);
	~ObjModel();
	void openFile(const char *fileName);
	void render();
};

}
#endif
