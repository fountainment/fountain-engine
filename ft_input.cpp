#include "ft_input.h"
#include "ft_data.h"

void ftInput::init()
{
}

using namespace ftInput;

Mouse::Mouse()
{
	Mouse::coor.x = 0;
	Mouse::coor.y = 0;
}

void Mouse::update(int x, int y)
{
	Mouse::coor.x = x;
	Mouse::coor.y = y;
	Mouse::coorNor.x = x;
	Mouse::coorNor.y = fountain::mainWin.h - y;
}

const ftVec2 & Mouse::getOriPos()
{
	return Mouse::coor;	
}

const ftVec2 & Mouse::getPos()
{
	return Mouse::coorNor;
}

