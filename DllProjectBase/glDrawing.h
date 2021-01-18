#pragma once
#include "includes.h"

namespace gl {
	void DrawSnaplines(float x1, float y1, float x2, float y2, int* RGB);
	void DrawBox(float x, float y, float width, float height, int* RGB);
	void DrawCross(float x, float y, float width, float height, float LineWidth, int* RGB);
	void DrawFilledBox(float x, float y, float width, float height, int* RGB);
	void DrawCircle(float cx, float cy, float r, int num_segments, int* RGB);
}