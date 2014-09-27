#ifndef _FT_3DMODEL_
#define _FT_3DMODEL_
#include "ft_data.h"

typedef struct {
	int a, b, c;
} ftVec3Index;

namespace ft3DModel
{

	void init();

	class ObjModel
	{
		private:
			int vecN;
			int indexN;
			ftVec3 v[100000];
			ftVec3Index p[100000];
		public:
			explicit ObjModel(const char *fileName);
			void openFile(const char *fileName);
			void render();
	};
}

#endif
