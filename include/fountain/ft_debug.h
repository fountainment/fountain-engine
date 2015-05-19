#ifndef _FT_DEBUG_H_
#define _FT_DEBUG_H_

#include <cstdio>
#include <cstdlib>

extern std::FILE *debugF;

#ifdef DEBUG
#define FT_OUT std::printf
#else
#define FT_OUT(...)
#endif

#ifndef NDEBUG
#define FT_LOG(...) std::fprintf(debugF, __VA_ARGS__)
#define FT_ERROR(...) FT_OUT(__VA_ARGS__);FT_LOG(__VA_ARGS__);std::exit(0)
#define FT_ASSERT(_STATE_,_STR_) if(!(_STATE_)){FT_ERROR("Assertion Error: "#_STR_" (("#_STATE_") return false!)\n");}
#else
#define FT_LOG(...)
#define FT_ERROR(...)
#define FT_ASSERT(_STATE_,_STR_)
#endif

namespace ftDebug {

bool init();
void close();

};

#endif
