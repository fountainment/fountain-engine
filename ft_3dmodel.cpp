#include <fountain/fountaindef.h>
#define GLEW_STATIC
#include <GL/glew.h>
#include <cstdio>
#include <cstring>
#include <vector>

using ft3DModel::ObjModel;

typedef struct {
	int i[3];
} ftVec3Index;

bool ft3DModel::init()
{
	return true;
}

void ft3DModel::close()
{
}

ObjModel::ObjModel()
{
	vtx = NULL;
	vtxN = NULL;
}

ObjModel::ObjModel(const char *fileName)
{
	loadObj(fileName);
}

ObjModel::~ObjModel()
{
	if (vtx != NULL) {
		delete [] vtx;
		vtx = NULL;
	}
	if (vtxN != NULL) {
		delete [] vtxN;
		vtxN = NULL;
	}
}

void ObjModel::loadObj(const char *fileName)
{
	char tmp[10];
	int tmpInt;
	std::vector<ftVec3> v, vn;
	std::vector<ftVec3Index> p, n;
	ftVec3 tmpV, tmpVN;
	ftVec3Index tmpP, tmpN;
	v.clear();
	vn.clear();
	p.clear();
	n.clear();
	v.push_back(tmpV);
	vn.push_back(tmpVN);
	std::FILE *objFile = std::fopen(fileName, "r");
	vecN = 1;
	indexN = 0;
	if (objFile != NULL) {
		for (;;) {
			if (std::fscanf(objFile, "%s", tmp) == EOF)
				break;
			if (std::strcmp(tmp, "v") == 0) {
				for (int i = 0; i < 3; i++) {
					tmpInt =
					    std::fscanf(objFile, "%f",
					                &tmpV.
					                xyz[i]);
					if (tmpInt != 1)
						break;
				}
				v.push_back(tmpV);
				vecN++;
			} else if (std::strcmp(tmp, "vn") == 0) {
				for (int i = 0; i < 3; i++) {
					tmpInt =
					    std::fscanf(objFile, "%f",
					                &tmpVN.
					                xyz[i]);
					if (tmpInt != 1)
						break;
				}
				vn.push_back(tmpVN);
			} else if (std::strcmp(tmp, "f") == 0) {
					for (int i = 0; i < 3; i++) {
						tmpInt = std::fscanf(objFile, "%d//%d", &tmpP.i[i], &tmpN.i[i]);
					}
					p.push_back(tmpP);
					n.push_back(tmpN);
					indexN++;
			}
			while (tmpInt =
			            std::fscanf(objFile, "%c", tmp), tmpInt != EOF
			        && tmp[0] != '\n');
		}
		std::fclose(objFile);
		vtx = new float[indexN * 3][3];
		vtxN = new float[indexN * 3][3];
		for (int tri = 0; tri < indexN; tri++) {
			for (int vti = 0; vti < 3; vti++) {
				for (int coori = 0; coori < 3; coori++) {
					vtx[tri * 3 + vti][coori] = v[p[tri].i[vti]].xyz[coori];
					vtxN[tri * 3 + vti][coori] = vn[n[tri].i[vti]].xyz[coori];
				}
			}
		}
	} else {
		FT_OUT("Open \"%s\" error!\n", fileName);
	}
}

void ObjModel::render()
{
	glEnable(GL_DEPTH_TEST);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vtx);
	glNormalPointer(GL_FLOAT, 3, vtxN);
	glDrawArrays(GL_TRIANGLES, 0, indexN * 3);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisable(GL_DEPTH_TEST);
}
