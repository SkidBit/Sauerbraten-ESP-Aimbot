#pragma once
#include "includes.h"
#include <objidl.h>
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")


namespace gdi {
	void DrawFilledRectangle(float x, float y, float width, float height, Pen& pen,  Graphics& graphics);
	void DrawBorderBox(int x, int y, int width, int height, int thickness, Pen& pen,  Graphics& graphics);
	void DrawRectangle(float x, float y, float width, float height, Pen& pen,  Graphics& graphics);
	void DrawLine(float StartX, float StartY, float EndX, float EndY, Pen& pen,  Graphics &graphics);
	void runSetup();
}