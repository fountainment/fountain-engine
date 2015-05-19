#include <fountain/fountaindef.h>
#include <cstdlib>
#include <cstring>

static const unsigned char lOne[] = {0x00, 0x80, 0xc0, 0xe0,
                                     0xf0, 0xf8, 0xfc, 0xfe, 0xff
                                    };
static const unsigned char rOne[] = {0x00, 0x01, 0x03, 0x07,
                                     0x0f, 0x1f, 0x3f, 0x7f, 0xff
                                    };

static unsigned int randomSeed;

bool ftAlgorithm::init()
{
	ftAlgorithm::setRandonSeed((unsigned)ftTime::getInitTime());
	return true;
}

void ftAlgorithm::close()
{
}

void ftAlgorithm::setRandonSeed(unsigned int rs)
{
	randomSeed = rs;
	std::srand(randomSeed);
}

unsigned int ftAlgorithm::getRandomSeed()
{
	return randomSeed;
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
	double zerone = (double)std::rand() / (double)RAND_MAX;
	zerone *= r - l;
	zerone += l;
	return zerone;
}

int firstZero(char c)
{
	int ans = 0;
	while (c & (1 << (7 - ans))) ans++;
	return ans;
}

std::vector<unsigned long> ftAlgorithm::utf8toUnicode(const char *input)
{
	std::vector<unsigned long> res;
	int len = std::strlen(input);
	int i = 0;
	unsigned char ucTmp;
	unsigned long ulTmp;
	while (i < len) {
		ucTmp = (unsigned char)input[i];
		int t = firstZero(ucTmp);
		switch (t) {
		/*
		case 1:
			FT_OUT("error\n");
			break;
		*/
		case 0:
			ulTmp = (unsigned long)ucTmp;
			res.push_back(ulTmp);
			i += 1;
			break;

		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
			ulTmp = (unsigned long)(ucTmp & rOne[7 - t]);
			for (int j = 1; j < t; j++) {
				ulTmp <<= 6;
				/*
				if (i + j >= len) {
					FT_OUT("error\n");
					break;
				}
				*/
				ucTmp = (unsigned char)input[i + j];
				ulTmp |= ucTmp & rOne[6];
			}
			res.push_back(ulTmp);
			i += t;
			break;
		}
	}
	return res;
}

int ftAlgorithm::nextPower2(int num)
{
	int rVal = 1;
	while (rVal < num) rVal <<= 1;
	return rVal;
}
