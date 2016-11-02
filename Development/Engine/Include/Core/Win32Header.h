#pragma once

#ifdef TEXT
#undef TEXT
#endif


#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include <WindowsX.h>
#include <zmouse.h>

#ifndef WM_XBUTTONDOWN
#define WM_XBUTTONDOWN 0x020B // (not always defined)
#endif
#ifndef WM_XBUTTONUP
#define WM_XBUTTONUP 0x020C // (not always defined)
#endif

#define WM_XBUTTONDBLCLK                0x020D

#undef DrawText // DrawText ³åÍ»

