#include "ft_data.h"

namespace fountain {
	winState mainWin;
}
const ftVec2 operator-(const ftVec2 & a, const ftVec2 & b)
{
	ftVec2 ans;
	ans.x = a.x - b.x;
	ans.y = a.y - b.y;
	return ans;
}

const ftVec2 operator+(const ftVec2 & a, const ftVec2 & b)
{
	ftVec2 ans;
	ans.x = a.x + b.x;
	ans.y = a.y + b.y;
	return ans;
}
