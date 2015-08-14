#include "polygon.h"
#include <algorithm>
#include <iostream>

using fim::Polygon;
using fim::PolygonList;
using fim::PolygonQueue;

PolygonList Polygon::convexDecomposition()
{
	PolygonList res;
	fim::PolygonQueue processQueue;
	processQueue.push(regular());
	while(!processQueue.empty()) {
		auto currentPoly = processQueue.front();
		processQueue.pop();
		int ccPoint = currentPoly.getConcavePoint();
		if (currentPoly.size() >= 3) {
			if (ccPoint == -1) {
				res.push_back(currentPoly);
			} else {
				auto twoPolygon = currentPoly.cutPolygon(ccPoint);
				for (int i = 0; i < (int)twoPolygon.size(); i++) {
					auto tmpPoly = twoPolygon[i].regular();
					if (tmpPoly.size() >= 3) {
						processQueue.push(tmpPoly);
					}
				}
			}
		}
		if (res.size() > 100 || processQueue.size() > 100) {
			break;
		}
	}
	return res;
}

const Polygon Polygon::regular()
{
	Polygon res;
	for (int i = 0; i < (int)size(); i++) {
		if (getPoint(i) != getPoint(i - 1)
		&& getVector(i).normalize() != getVector(i - 1).normalize()) {
			res.push_back(getPoint(i));
		}
	}
	return res;
}

void Polygon::print()
{
	std::cout << size() << std::endl;
	for (auto it = begin(); it != end(); ++it) {
		std::cout << (*it).x << " " << (*it).y << std::endl;
	}
}

int Polygon::indexNormalize(int index)
{
	int sz = (int)size();
	return ((index % sz) + sz) % sz;
}

int Polygon::insertPoint(const vec2 & p)
{
	int ans = 0;
	fim::vec2 lastVec;
	fim::segment seg;
	for (Polygon::iterator it = begin(); it != end(); ++it) {
		if (it != begin()) {
			seg = fim::segment(lastVec, *it);
			if (seg.collidePoint(p)) {
				ans = std::distance(begin(), insert(it, p));
				break;
			}
		}
		lastVec = *it;
	}
	if (ans == 0) insert(begin(), p);
	return ans;
}

const fim::vec2 Polygon::getPoint(int index)
{
	index = indexNormalize(index);
	return (*this)[index];
}

const fim::vec2 Polygon::getVector(int index)
{
	return getPoint(index + 1) - getPoint(index);
}

const fim::segment Polygon::getSegment(int index)
{
	return fim::segment(getPoint(index), getPoint(index + 1));
}

int Polygon::minDistanceOfTwoPoint(int pa, int pb)
{
	int ans = pa - pb;
	int sz = size();
	if (ans < 0) ans *= -1;
	if (ans > sz / 2) ans = sz - ans;
	return ans;
}

bool Polygon::isConcavePoint(int index)
{
	auto prev = getVector(index - 1);
	auto cur = getVector(index);
	double cp = prev.crossProduct(cur);
	if (cp < 0) return true;
	else return false;
}

int Polygon::getConcavePoint()
{
	int ans = -1;
	int sz = (int)size();
	for (int i = 0; i < sz; i++) {
		if (isConcavePoint(i)) {
			ans = i;
			break;
		}
	}
	return ans;
}

static fim::vec2 raySrc;
bool cmpDistanceToSrc(const fim::vec2 & a, const fim::vec2 & b)
{
	return (a - raySrc).length() < (b - raySrc).length();
}

fim::Point2List Polygon::collideRay(const fim::vec2 & src, const fim::vec2 & drct)
{
	fim::Point2List res;
	fim::Point2List uniqueRes;
	for (int i = 0; i < (int)size(); i++) {
		auto seg = getSegment(i);
		fim::vec2 tmp; 
		if (seg.collideRay(tmp, src, drct)) {
			res.push_back(tmp);
		}
	}
	raySrc = src;
	std::sort(res.begin(), res.end(), cmpDistanceToSrc);
	for (int i = 0; i < (int)res.size(); i++) {
		if (res[i] == src) continue;
		if (i == 0 || res[i] != res[i - 1]) {
			uniqueRes.push_back(res[i]);
		}
	}
	return uniqueRes;
}

bool Polygon::isBetterPoint(int ccPoint, int pa, int pb)
{
	if (pb == -1) return true;
	if (minDistanceOfTwoPoint(pa, ccPoint) < 
	 minDistanceOfTwoPoint(pb, ccPoint)) {
		return true;
	}
	return false;
}

fim::IndexList Polygon::getVisiblePointIndex(int ccPoint)
{
	fim::IndexList res;
	auto left = getSegment(ccPoint);
	auto right = getSegment(ccPoint - 1);
	right.swapAB();
	left.b = left.a - left.getVector();
	right.b = right.a - right.getVector();
	for (int i = 0; i < (int)size(); i++) {
		if (i == ccPoint) continue;
		if (i == indexNormalize(ccPoint + 1)) continue;
		if (i == indexNormalize(ccPoint - 1)) continue;
		auto p = getPoint(i);
		if (left.leftOrRight(p) == 1
			&& right.leftOrRight(p) == -1) {
			auto pl = collideRay(getPoint(ccPoint), p - getPoint(ccPoint));
			if (pl.size() == 0 || pl[0] == p)
			{
				res.push_back(i);
			}
		}
	}
	return res;
}

int Polygon::getBestPointIndex(int ccPoint)
{
	int index = -1;
	int state = 0;
	auto iList = getVisiblePointIndex(ccPoint);
	for (auto it = iList.begin(); it != iList.end(); ++it) {
		if (state < 4 && isConcavePoint(*it) && isBetterPoint(ccPoint, *it, index)) {
			index = *it;
			state = 4;
		} else if (state < 3 && isConcavePoint(*it)) {
			index = *it;
			state = 3;
		} else if (state < 2 && isBetterPoint(ccPoint, *it, index)) {
			index = *it;
			state = 2;
		} else if (state < 1) {
			index = *it;
			state = 1;
		}
	}
	return index;
}

PolygonList Polygon::cutPolygon(int ccPoint, int anoPoint)
{
	PolygonList res;
	Polygon tmp;
	int cur = anoPoint;
	tmp.push_back(getPoint(ccPoint));
	tmp.push_back(getPoint(anoPoint));
	cur = indexNormalize(cur + 1);
	while (cur != ccPoint) {
		tmp.push_back(getPoint(cur));
		cur = indexNormalize(cur + 1);
	}
	res.push_back(tmp);
	tmp.clear();
	cur = ccPoint;
	tmp.push_back(getPoint(anoPoint));
	tmp.push_back(getPoint(ccPoint));
	cur = indexNormalize(cur + 1);
	while (cur != anoPoint) {
		tmp.push_back(getPoint(cur));
		cur = indexNormalize(cur + 1);
	}
	res.push_back(tmp);
	return res;
}

PolygonList Polygon::cutPolygon(int ccPoint)
{
	PolygonList res;
	int best = getBestPointIndex(ccPoint);
	if (best != -1) {
		return cutPolygon(ccPoint, best);
	} else {
		auto left = -getVector(ccPoint);
		auto right = getVector(ccPoint - 1);
		auto middleVec = (left.normalize() + right.normalize()) / 2.0; 
		auto pl = collideRay(getPoint(ccPoint), middleVec);
		if (!pl.empty()) {
			int loc = insertPoint(pl[0]);
			if (loc <= ccPoint) {
				ccPoint = indexNormalize(ccPoint + 1);
			}
			return cutPolygon(ccPoint, loc);
		} else {
			//TODO: debug output
		}
	}
	return res;
}

