#ifndef _FT_DEBUG_H_
#define _FT_DEBUG_H_

#include <cstdio>
#include <cstring>

#ifdef DEBUG
	#define FT_OUT std::printf
	#define FT_LOG(...) std::fprintf(debugF, __VA_ARGS__) 
#else
	#define FT_OUT(...)
	#define FT_LOG(...) 
#endif

namespace ftDebug {

bool init();
void close();

};

#endif
