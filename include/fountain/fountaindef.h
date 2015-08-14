#ifndef _FOUNTAINDEF_H_
#define _FOUNTAINDEF_H_

#define FOUNTAIN_VERSION "0.070"
#define FOUNTAIN_VERSION_NUM 0.070

#define FT_LButton 1
#define FT_MButton 2
#define FT_RButton 3
#define FT_ScrollUp 4
#define FT_ScrollDown 5

#define FT_isUp 0
#define FT_isDown 1

#define FT_KeyDown 2
#define FT_KeyUp 3

#define FT_ButtonDown 2
#define FT_ButtonUp 3

#define FT_None 0
#define FT_isOn 4

#define FT_A 0x0001
#define FT_B 0x0002
#define FT_C 0x0003
#define FT_D 0x0004
#define FT_E 0x0005
#define FT_F 0x0006
#define FT_G 0x0007
#define FT_H 0x0008
#define FT_I 0x0009
#define FT_J 0x000a
#define FT_K 0x000b
#define FT_L 0x000c
#define FT_M 0x000d
#define FT_N 0x000e
#define FT_O 0x000f
#define FT_P 0x0010
#define FT_Q 0x0011
#define FT_R 0x0012
#define FT_S 0x0013
#define FT_T 0x0014
#define FT_U 0x0015
#define FT_V 0x0016
#define FT_W 0x0017
#define FT_X 0x0018
#define FT_Y 0x0019
#define FT_Z 0x001a

#define FT_F1 0x001a
#define FT_F2 0x001b
#define FT_F3 0x001c
#define FT_F4 0x001d
#define FT_F5 0x001e
#define FT_F6 0x001f
#define FT_F7 0x0020
#define FT_F8 0x0021
#define FT_F9 0x0022
#define FT_F10 0x0023
#define FT_F11 0x0024
#define FT_F12 0x0025

#define FT_Esc 0x0026

#define FT_0 0x0027
#define FT_1 0x0028
#define FT_2 0x0029
#define FT_3 0x002a
#define FT_4 0x002b
#define FT_5 0x002c
#define FT_6 0x002d
#define FT_7 0x002e
#define FT_8 0x002f
#define FT_9 0x0030

#define FT_Enter 0x0031
#define FT_Space 0x0032
#define FT_Ctrl 0x0033
#define FT_Alt 0x0034
#define FT_Shift 0x0035
#define FT_Up 0x0036
#define FT_Down 0x0037
#define FT_Left 0x0038
#define FT_Right 0x0039

#define FT_ON 1
#define FT_OFF 0

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cmath>

#include <algorithm>
#include <vector>
#include <stack>
#include <queue>
#include <map>

#include <fountain/ft_math.h>
#include <fountain/ft_data.h>
#include <fountain/ft_time.h>
#include <fountain/ft_3dmodel.h>
#include <fountain/ft_render.h>
#include <fountain/ft_physics.h>
#include <fountain/ft_input.h>
#include <fountain/ft_algorithm.h>
#include <fountain/ft_scene.h>
#include <fountain/ft_type.h>
#include <fountain/ft_audio.h>
#include <fountain/ft_ui.h>
#include <fountain/ft_anime.h>
#include <fountain/ft_debug.h>

namespace fountain {

void checkSysState();
void readSettingFile();

void basicSetting();

void initAllSystem();
void closeAllSystem();

//these three functions are what you should implement
void setBasicVarible();
void gameInit();
void singleFrame();

ftVec2 getWinSize();
ftRect getWinRect();
ftTime::Clock* getCurSceneClock();
ftRender::Camera* getCurSceneCamera();
float getCurSceneDeltaT();

}

#endif
