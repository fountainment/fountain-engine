#include "ft_input.h"
#include "ft_data.h"
#include <cstring>

void ftInput::init()
{
}

using namespace ftInput;

Mouse::Mouse()
{
	Mouse::coor.x = 0;
	Mouse::coor.y = 0;
	Mouse::coorNor = Mouse::coor;
	Mouse::lastCoorNor = Mouse::coorNor;
}

void Mouse::update(int x, int y)
{
	Mouse::lastCoorNor = Mouse::coorNor;
	Mouse::coor.x = x;
	Mouse::coor.y = y;
	Mouse::coorNor.x = x;
	Mouse::coorNor.y = fountain::mainWin.h - 1 - y;
}

const ftVec2 & Mouse::getOriPos()
{
	return Mouse::coor;
}

const ftVec2 & Mouse::getPos()
{
	return Mouse::coorNor;
}

const ftVec2 Mouse::getDeltaV()
{
	ftVec2 ans = Mouse::coorNor - Mouse::lastCoorNor;
	Mouse::lastCoorNor = Mouse::coorNor;
	return ans;
}

void Mouse::setState(int button, int st)
{
	Mouse::state[button] = st;
}

int Mouse::getState(int button)
{
	return Mouse::state[button];
}

void Mouse::clearState()
{
	std::memset(state, 0, sizeof(state));
}

Keyboard::Keyboard()
{
	std::memset(state, 0, sizeof(state));
}

void Keyboard::setState(int ch, int st)
{
	Keyboard::state[ch] = st;
}

int Keyboard::getState(int ch)
{
	return Keyboard::state[ch];
}

void Keyboard::clearState()
{
	std::memset(state, 0, sizeof(state));
}

namespace fountain {
	ftInput::Mouse sysMouse;
	ftInput::Keyboard sysKeyboard;
}
