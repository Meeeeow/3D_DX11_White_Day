#pragma once
#ifndef __CLIENT_DEFINES_H__
#define __CLIENT_DEFINES_H__



static	unsigned	int	g_iWinCX = 1020;
static	unsigned	int	g_iWinCY = 800;

enum	LEVEL		{ LEVEL_STATIC, LEVEL_DYNAMIC, LEVEL_LOADING, LEVEL_LOGO, LEVEL_QASMOKE, LEVEL_MAZE, LEVEL_END };
enum	CAMERAS		{ CAMERA_DEFAULT, CAMERA_END };

extern	HWND		g_hWnd;
extern	HINSTANCE	g_hInstance;

#include "Client_Struct.h"
#include "Defines_Component.h"
#include "Defines_Object.h"
#include "Defines_Layer.h"

using namespace std;
using namespace Client;

#endif // !__CLIENT_DEFINES_H__
