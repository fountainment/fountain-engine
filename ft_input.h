#ifndef FT_INPUT_H_
#define FT_INPUT_H_
#include "ft_data.h"

#define FT_KEYBOARDSTATE_SIZE 512
#define FT_MOUSESTATE_SIZE 5

namespace ftInput
{
	void init();

	class Mouse
	{
		private:
			ftVec2 coor;
			ftVec2 lastCoorNor;
			ftVec2 coorNor;
			int state[FT_MOUSESTATE_SIZE];
		public:
			Mouse();
			void update(int x, int y);
			const ftVec2 & getPos();
			const ftVec2 & getOriPos();
			const ftVec2 getDeltaV();
			void setState(int button, int st);
			int getState(int button);
			void clearState();
	};

	class Keyboard
	{
		private:
			int state[FT_KEYBOARDSTATE_SIZE];
		public:
			Keyboard();
			void setState(int ch, int st);
			int getState(int ch);
			void clearState();
	};
}

namespace fountain
{
	extern ftInput::Mouse sysMouse;
	extern ftInput::Keyboard sysKeyboard;
}

#endif
