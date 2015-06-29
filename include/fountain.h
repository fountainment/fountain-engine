#ifndef _FOUNTAIN_H_
#define _FOUNTAIN_H_

#include <fountain/fountaindef.h>

#ifndef FT_Console

#ifdef __linux
#include <fountain/linFountain.h>
#endif

#ifdef _WIN32
#include <fountain/winFountain.h>
#endif

#else
//console
void fountain::setBasicVarible()
{
}

void fountain::gameInit()
{
}

void fountain::singleFrame()
{
}
#endif

#endif
