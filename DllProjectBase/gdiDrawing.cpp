#include "gdiDrawing.h"

void gdi::DrawBorderBox(int x, int y, int width, int height, int thickness, Pen& pen, Graphics& graphics)
{
	gdi::DrawFilledRectangle(x, y, width, thickness, pen, graphics);
	gdi::DrawFilledRectangle(x, y, thickness, height, pen, graphics);
	gdi::DrawFilledRectangle((x + width), y, thickness, height, pen, graphics);
	gdi::DrawFilledRectangle(x, y + height, width + thickness, thickness, pen, graphics);
}

void gdi::DrawFilledRectangle(float x, float y, float width, float height, Pen &pen, Graphics& graphics)
{
	graphics.DrawRectangle(&pen, x, y, x + width, y + height);
}

void gdi::DrawRectangle(float x, float y, float width, float height, Pen &pen, Graphics& graphics)
{
	graphics.DrawRectangle(&pen, x, y, width, height);
}

void gdi::DrawLine(float StartX, float StartY, float EndX, float EndY, Pen &pen, Graphics &graphics)
{
	graphics.DrawLine(&pen, StartX, StartY, EndX, EndY);
}

void gdi::runSetup() {
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
}