#include <fountain/ft_debug.h>
#include <cstdlib>

std::FILE *debugF;

bool ftDebug::init()
{
	debugF = std::fopen("log.txt", "w");
	return debugF != NULL;
}

void ftDebug::close()
{
	std::fclose(debugF);
}
