#ifndef _FIM_SEGMENT_H_
#define _FIM_SEGMENT_H_

#include "vec2.h"

namespace fim {

class segment
{
public:
	segment();
	segment(vec2 pa, vec2 pb);

	void swapAB();

	const vec2 getVector() const;
	double length() const;

	bool rectCollidePoint(const vec2 & p) const;
	bool collidePoint(const vec2 & p) const;
	bool collideSegment(vec2 & pt, const segment & seg) const;
	bool collideRay(vec2 & pt, const vec2 & src, const vec2 & drct);

	int leftOrRight(const vec2 & pt) const;
	/* -1 Left0 On 1 Right */

	vec2 a, b;
};

}

#endif
