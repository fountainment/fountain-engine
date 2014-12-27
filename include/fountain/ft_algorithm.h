#ifndef _FT_ALGORITHM_H_
#define _FT_ALGORITHM_H_

#include <vector>

#define FT_Pi 3.14159f

namespace ftAlgorithm {

void init();
int bkdrHash(const char *str);
float randRangef(float l, float r);

std::vector<unsigned long> utf8toUnicode(const char *input);

};

#endif
