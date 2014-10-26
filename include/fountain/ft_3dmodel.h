#ifndef _FT_3DMODEL_
#define _FT_3DMODEL_
#include <fountain/ft_data.h>

typedef struct {
	int a, b, c;
} ftVec3Index;

namespace ft3DModel {

	void init();

	class ObjModel {
private:
		int vecN;
		int indexN;
		ftVec3 v[50000];
		ftVec3Index p[50000];
		int neverRendered;
		int listIndex;
public:
		 explicit ObjModel(const char *fileName);
		void openFile(const char *fileName);
		void render();
	};
}
#endif
