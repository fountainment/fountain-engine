#include <fountain/ft_input.h>
#include <fountain/ft_data.h>
#include <cstring>

using ftInput::Mouse;
using ftInput::Keyboard;

void ftInput::init()
{
}

Mouse::Mouse()
{
	Mouse::clearState();
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
	std::memset(Mouse::state, 0, sizeof(Mouse::state));
}

Keyboard::Keyboard()
{
	Keyboard::clearState();
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
	std::memset(Keyboard::state, 0, sizeof(Keyboard::state));
}

namespace fountain {

ftInput::Mouse sysMouse;
ftInput::Keyboard sysKeyboard;

};
