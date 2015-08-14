#include "segment.h"
#include "const.h"
#include <cmath>

using fim::segment;

segment::segment()
{
}

segment::segment(fim::vec2 pa, fim::vec2 pb)
:a(pa),
 b(pb)
{
}

void segment::swapAB()
{
	vec2 tmp = a;
	a = b;
	b = tmp;
}

const fim::vec2 segment::getVector() const
{
	return (b - a);
}

double segment::length() const
{
	return getVector().length();
}

bool segment::rectCollidePoint(const fim::vec2 & p) const
{
	return ((p.x - a.x) * (p.x - b.x) <= 0
		&& (p.y - a.y) * (p.y - b.y) <= 0);
}

bool segment::collidePoint(const fim::vec2 & p) const
{
	return (std::abs((p - a).crossProduct(b - a)) < fim::eps) && rectCollidePoint(p);
}

bool segment::collideSegment(vec2 & pt, const segment & seg) const
{
	fim::vec2 pa(a), pb(b), pc(seg.a), pd(seg.b), intersection;
	intersection.x = ((pb.x - pa.x) * (pc.x - pd.x) * (pc.y - pa.y) -
	pc.x * (pb.x - pa.x) * (pc.y - pd.y) + pa.x * (pb.y - pa.y) * (pc.x - pd.x)) /
	((pb.y - pa.y) * (pc.x - pd.x) - (pb.x - pa.x) * (pc.y - pd.y));
	intersection.y = ((pb.y - pa.y) * (pc.y - pd.y) * (pc.x - pa.x) - pc.y
	* (pb.y - pa.y) * (pc.x - pd.x) + pa.y * (pb.x - pa.x) * (pc.y - pd.y))
	/ ((pb.x - pa.x) * (pc.y - pd.y) - (pb.y - pa.y) * (pc.x - pd.x));
	if (rectCollidePoint(intersection) && seg.rectCollidePoint(intersection)) {
		pt = intersection;
		return true;
	} else {
		return false;
	}
}

bool segment::collideRay(vec2 & pt, const vec2 & src, const vec2 & drct)
{
	fim::vec2 pa(a), pb(b), pc(src), pd(src + drct), intersection;
	intersection.x = ((pb.x - pa.x) * (pc.x - pd.x) * (pc.y - pa.y) -
	pc.x * (pb.x - pa.x) * (pc.y - pd.y) + pa.x * (pb.y - pa.y) * (pc.x - pd.x)) /
	((pb.y - pa.y) * (pc.x - pd.x) - (pb.x - pa.x) * (pc.y - pd.y));
	intersection.y = ((pb.y - pa.y) * (pc.y - pd.y) * (pc.x - pa.x) - pc.y
	* (pb.y - pa.y) * (pc.x - pd.x) + pa.y * (pb.x - pa.x) * (pc.y - pd.y))
	/ ((pb.x - pa.x) * (pc.y - pd.y) - (pb.y - pa.y) * (pc.x - pd.x));
	if (rectCollidePoint(intersection) && drct.dotProduct(intersection - src) > 0) {
		pt = intersection;
		return true;
	} else {
		return false;
	}
}

int segment::leftOrRight(const fim::vec2 & pt) const
{
	int ans = 0;
	fim::vec2 vec = getVector();
	fim::vec2 p = pt - a;
	double cp = vec.crossProduct(p);
	if (cp > 0) ans = -1;
	else if (cp < 0) ans = 1;
	return ans;
}
