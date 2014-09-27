#ifndef FT_INPUT_H_
#define FT_INPUT_H_
#include "ft_data.h"

namespace ftInput
{
	void init();

	class Mouse
	{
		private:
			ftVec2 coor;
			ftVec2 coorNor;
		public:
			Mouse();
			void update(int x, int y);
			const ftVec2 & getPos();
			const ftVec2 & getOriPos();
	};	
}

#endif
