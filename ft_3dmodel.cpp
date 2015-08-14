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
	glGenBuffers(1, &vboV);
	glGenBuffers(1, &vboN);
}

ObjModel::ObjModel(const char *fileName)
{
	loadObj(fileName);
}

ObjModel::~ObjModel()
{
	glDeleteBuffers(1, &vboV);
	glDeleteBuffers(1, &vboN);
}

void ObjModel::loadObj(const char *fileName, bool smooth)
{
	GLfloat (*vtx)[3] = NULL;
	GLfloat (*vtxN)[3] = NULL;
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
		vtx = new GLfloat[indexN * 3][3];
		vtxN = new GLfloat[indexN * 3][3];
		std::map<ftVec3, ftVec3> mapVertex2Normal;
		std::map<ftVec3, int> mapVertex2NormalNum;
		for (int tri = 0; tri < indexN; tri++) {
			for (int vti = 0; vti < 3; vti++) {
				ftVec3 vertex = v[p[tri].i[vti]];
				ftVec3 normal = vn[n[tri].i[vti]];
				vertex.output(vtx[tri * 3 + vti]);
				if (smooth) {
					mapVertex2Normal[vertex] += normal;
					mapVertex2NormalNum[vertex]++;
				} else normal.output(vtxN[tri * 3 + vti]);
			}
		}
		if (smooth) {
			for (int tri = 0; tri < indexN; tri++) {
				for (int vti = 0; vti < 3; vti++) {
					ftVec3 vertex = v[p[tri].i[vti]];
					ftVec3 normal = mapVertex2Normal[vertex] / mapVertex2NormalNum[vertex];
					normal.output(vtxN[tri * 3 + vti]);
				}
			}
		}
	} else {
		FT_OUT("Open \"%s\" error!\n", fileName);
	}

	glBindBuffer(GL_ARRAY_BUFFER, vboV);
	glBufferData(GL_ARRAY_BUFFER, indexN * 9 * sizeof(GLfloat), vtx, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vboN);
	glBufferData(GL_ARRAY_BUFFER, indexN * 9 * sizeof(GLfloat), vtxN, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	if (vtx != NULL) {
		delete [] vtx;
		vtx = NULL;
	}
	if (vtxN != NULL) {
		delete [] vtxN;
		vtxN = NULL;
	}
}

void ObjModel::render()
{
	glEnable(GL_DEPTH_TEST);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, vboV);
	glVertexPointer(3, GL_FLOAT, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, vboN);
	glNormalPointer(GL_FLOAT, 0, 0);

	glDrawArrays(GL_TRIANGLES, 0, indexN * 3);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDisable(GL_DEPTH_TEST);
}
