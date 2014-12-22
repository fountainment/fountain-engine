#include <fountain/ft_type.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#include <cstdio>

FT_Library library;

bool ftType::init()
{
	int error = FT_Init_FreeType( &library );
	if (error) {
		return false;
	}
	return true;
}
