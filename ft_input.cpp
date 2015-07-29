#include <fountain/fountaindef.h>
#include <cstring>
#include <stack>

using ftInput::Mouse;
using ftInput::Keyboard;

static std::stack<int> buttonChangeList;
static std::stack<int> keyChangeList;

bool ftInput::init()
{
	return true;
}

void ftInput::close()
{
}

void ftInput::initPerFrame()
{
	int tmp;
	int state;
	fountain::sysMouse.setState(FT_ScrollUp, 0);
	fountain::sysMouse.setState(FT_ScrollDown, 0);
	fountain::sysKeyboard.asciiIn = 0;
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

//class ftInput::Mouse
Mouse::Mouse()
{
	clearState();
	coor.x = 0;
	coor.y = 0;
	coorNor = Mouse::coor;
	lastCoorNor = Mouse::coorNor;
}

void Mouse::update(int x, int y)
{
	lastCoorNor = Mouse::coorNor;
	coor.x = x;
	coor.y = y;
	coorNor.x = x;
	coorNor.y = fountain::mainWin.h - 1 - y;
}

const ftVec2 & Mouse::getOriPos()
{
	return coor;
}

const ftVec2 & Mouse::getPos()
{
	return coorNor;
}

const ftVec2 Mouse::getDeltaV()
{
	ftVec2 ans = coorNor - lastCoorNor;
	lastCoorNor = coorNor;
	return ans;
}

void Mouse::setState(int button, int st)
{
	state[button] = st;
	buttonChangeList.push(button);
}

int Mouse::getState(int button)
{
	return state[button];
}

void Mouse::clearState()
{
	std::memset(state, 0, sizeof(state));
}

//class ftInput::Keyboard
Keyboard::Keyboard()
{
	clearState();
}

void Keyboard::setState(int ch, int st)
{
	state[ch] = st;
	keyChangeList.push(ch);
}

int Keyboard::getState(int ch)
{
	return state[ch];
}

char Keyboard::getAsciiInput()
{
        return asciiIn;
}

void Keyboard::clearState()
{
	std::memset(state, 0, sizeof(state));
}

namespace fountain {

ftInput::Mouse sysMouse;
ftInput::Keyboard sysKeyboard;

};
