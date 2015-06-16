#ifndef __CONFIG_H__
#define __CONFIG_H__


const int g_width = 1024;
const int g_height = 768;
const int CAM_MOVESPEED = 20;
HWND g_hwnd = 0;


float g_currentTime;												  //当前时间
float g_deltaTime;                                                  //时间差

POINT  g_currentMousePoint;

#define KEY_DOWN(vk_code) (GetAsyncKeyState(vk_code) & 0x8000 ? 1 : 0)
#define KEY_UP(vk_code) (GetAsyncKeyState(vk_code) & 0x8000 ? 0 : 1)


#endif
