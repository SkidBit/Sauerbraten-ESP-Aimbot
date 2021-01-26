#pragma once
#include "includes.h"
#include "glDrawing.h"
#include "esp.h"

inline HWND GameHWND;
inline HWND EspHWND;
inline LPCSTR WName = "Overlay 0.0.5";
inline HDC MainHDC;
inline HGLRC MainHGLRC;
inline RECT WindowRect;
inline RECT ClientRect;
inline float SWidth;
inline float SHeight;
inline HINSTANCE CurrentInstance;
inline int colorRed[3] = { 255, 0, 0 };
inline int colorGreen[3] = { 0, 255, 0 };
inline int colorGrey[3] = { 100, 100, 100 };

namespace overlay {
	void ResizeWindow();
	void Initiate();
	void SetupGL();
	LRESULT APIENTRY WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	DWORD WINAPI RedrawLoop(LPVOID PARAMS);
	DWORD WINAPI OverlayThread(LPVOID param);
}