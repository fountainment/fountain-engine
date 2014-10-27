#include <fountain/ft_algorithm.h>
#include <cstdlib>

void ftAlgorithm::init()
{
}

int ftAlgorithm::bkdrHash(const char *str)
{
	int seed = 131;
	int hash = 0;
	while (*str) {
		hash = hash * seed + (*str++);
	}
	return (hash & 0x7fffffff);
}

float ftAlgorithm::randRangef(float l, float r)
{
	float zerone = (float)std::rand() / (float)RAND_MAX;
	zerone *= r - l;
	zerone += l;
	return zerone;
}
