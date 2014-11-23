#include <fountain/ft_3dmodel.h>
#include <cstdio>
#include <GL/gl.h>

using ft3DModel::ObjModel;

void ft3DModel::init()
{
}

ObjModel::ObjModel(const char *fileName)
{
	openFile(fileName);
	neverRendered = 1;
}

void ObjModel::openFile(const char *fileName)
{
	char tmp;
	int tmpInt;
	std::FILE * objFile = std::fopen(fileName, "rb");
	vecN = 1;
	indexN = 0;
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
					                &v[vecN].
					                xyz[i]);
					if (tmpInt != 1)
						break;
				}
				vecN++;
			} else {
				if (tmp == 'f') {
					tmpInt =
					    std::fscanf(objFile, "%d%d%d",
					                &p[indexN].a,
					                &p[indexN].b,
					                &p[indexN].c);
					if (tmpInt != 3)
						continue;
					indexN++;
				}
			}
		}
		std::fclose(objFile);
		neverRendered = 1;
	} else {
		std::printf("Open \"%s\" error!\n", fileName);
	}
}

void ObjModel::render()
{
	//if (neverRendered) {
	//	listIndex = glGenLists(1);
	//	glNewList(listIndex, GL_COMPILE);
	for (int i = 0; i < indexN; i++) {
		glBegin(GL_LINE_LOOP);
		glVertex3fv(v[p[i].a].xyz);
		glVertex3fv(v[p[i].b].xyz);
		glVertex3fv(v[p[i].c].xyz);
		glEnd();
	}
	//	glEndList();
	//	neverRendered = 0;
	//} else {
	//	glCallList(listIndex);
	//}
}
