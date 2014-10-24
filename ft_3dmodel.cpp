#include "ft_3dmodel.h"
#include <cstdio>
#include <GL/gl.h>

void ft3DModel::init()
{
}

using namespace ft3DModel;

ObjModel::ObjModel(const char *fileName)
{
	ObjModel::openFile(fileName);
	ObjModel::neverRendered = 1;
}

void ObjModel::openFile(const char *fileName)
{
	char tmp;
	int tmpInt;
	std::FILE * objFile = std::fopen(fileName, "rb");
	ObjModel::vecN = 1;
	ObjModel::indexN = 0;
	if (objFile != NULL) {
		for (;;) {
			while (tmpInt =
			       std::fscanf(objFile, "%c", &tmp), tmpInt != EOF
			       && tmp != '\n') ;
			if (std::fscanf(objFile, "%c", &tmp) == EOF)
				break;
			if (tmp == 'v') {
				for (int i = 0; i < 3; i++) {
					tmpInt =
					    std::fscanf(objFile, "%f",
							&ObjModel::
							v[vecN].xyz[i]);
					if (tmpInt != 1)
						break;
				}
				ObjModel::vecN++;
			} else {
				if (tmp == 'f') {
					tmpInt =
					    std::fscanf(objFile, "%d%d%d",
							&ObjModel::p[indexN].a,
							&ObjModel::p[indexN].b,
							&ObjModel::p[indexN].c);
					if (tmpInt != 3)
						continue;
					ObjModel::indexN++;
				}
			}
		}
		std::fclose(objFile);
		ObjModel::neverRendered = 1;
	} else {
		std::printf("Open \"%s\" error!\n", fileName);
	}
}

void ObjModel::render()
{
	if (ObjModel::neverRendered) {
		ObjModel::listIndex = glGenLists(1);
		glNewList(ObjModel::listIndex, GL_COMPILE);
		for (int i = 0; i < ObjModel::indexN; i++) {
			glBegin(GL_LINE_LOOP);
			glVertex3fv(ObjModel::v[p[i].a].xyz);
			glVertex3fv(ObjModel::v[p[i].b].xyz);
			glVertex3fv(ObjModel::v[p[i].c].xyz);
			glEnd();
		}
		glEndList();
		ObjModel::neverRendered = 0;
	} else {
		glCallList(ObjModel::listIndex);
	}
}
