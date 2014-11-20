#include <fountain/ft_input.h>
#include <fountain/ft_data.h>
#include <cstring>
#include <stack>

using ftInput::Mouse;
using ftInput::Keyboard;

static std::stack<int> buttonChangeList;
static std::stack<int> keyChangeList;

void ftInput::init()
{
}

void ftInput::initPerFrame()
{
	int tmp;
	int state;
	fountain::sysMouse.setState(FT_ScrollUp, 0);
	fountain::sysMouse.setState(FT_ScrollDown, 0);
	while (!buttonChangeList.empty()) {
		tmp = buttonChangeList.top();
		state = fountain::sysMouse.getState(tmp);
		if (state == FT_ButtonDown)
			fountain::sysMouse.setState(tmp, FT_isDown);
		if (state == FT_ButtonUp)
			fountain::sysMouse.setState(tmp, FT_isUp);
		buttonChangeList.pop();
	}
	while (!keyChangeList.empty()) {
		tmp = keyChangeList.top();
		state = fountain::sysKeyboard.getState(tmp);
		if (state == FT_KeyDown)
			fountain::sysKeyboard.setState(tmp, FT_isDown);
		if (state == FT_KeyUp)
			fountain::sysKeyboard.setState(tmp, FT_isUp);
		keyChangeList.pop();
	}
}

//Mouse
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
	buttonChangeList.push(button);
}

int Mouse::getState(int button)
{
	return Mouse::state[button];
}

void Mouse::clearState()
{
	std::memset(Mouse::state, 0, sizeof(Mouse::state));
}

//Keyboard
Keyboard::Keyboard()
{
	Keyboard::clearState();
}

void Keyboard::setState(int ch, int st)
{
	Keyboard::state[ch] = st;
	keyChangeList.push(ch);
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
