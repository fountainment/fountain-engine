#ifndef _FT_ALGORITHM_H_
#define _FT_ALGORITHM_H_

#include <vector>

namespace ftAlgorithm {

bool init();
void close();

void setRandonSeed(unsigned int rs);
unsigned int getRandomSeed();

int bkdrHash(const char *str);
float randRangef(float l, float r);

std::vector<unsigned long> utf8toUnicode(const char *input);
int nextPower2(int num);

}

#endif
