#include "glDrawing.h"

void gl::DrawSnaplines(float x1, float y1, float x2, float y2, int* RGB)
{
	glLineWidth(3);
	glBegin(GL_LINES); //begin drawing snapline outline
	glColor4f(0, 0, 0, 1); //black, yes alpha
	glVertex2f(x1, y1); //enemies feet
	glVertex2f(x2 / 2, y2); //half screen width, bottom of screen
	glEnd();
	glLineWidth(1);
	glBegin(GL_LINES); //begin drawing color snapline
	glColor4f(RGB[0], RGB[1], RGB[2], 1); //color, yes alpha
	glVertex2f(x1, y1); //enemies feet
	glVertex2f(x2 / 2, y2); //half screen width, bottom of screen
	glEnd();
}

void gl::DrawBox(float x, float y, float width, float height, int* RGB)
{
    glLineWidth(3);
    glBegin(GL_LINE_LOOP); //begin drawing outline rect
    glColor4f(0, 0, 0, 1); //black, yes alpha
    glVertex2f(x - (width / 2), y); // bottom left
    glVertex2f(x - (width / 2), y - height); //top left
    glVertex2f(x + (width / 2), y - height); //top right
    glVertex2f(x + (width / 2), y); //bottom right
    glEnd();
    glLineWidth(1);
    glBegin(GL_LINE_LOOP);//begin drawing color rect
    glColor4f(RGB[0], RGB[1], RGB[2], 1);//color, yes alpha
    glVertex2f(x - (width / 2), y); // bottom left
    glVertex2f(x - (width / 2), y - height); //top left
    glVertex2f(x + (width / 2), y - height); //top right
    glVertex2f(x + (width / 2), y); //bottom right
    glEnd();
}

void gl::DrawCross(float x, float y, float width, float height, float LineWidth, int* RGB)
{
    //draw black outline cross
    glLineWidth(LineWidth + 2);
    glBegin(GL_LINES);
    glColor4f(0, 0, 0, 1); //black, yes alpha
    glVertex2f(x, y + height + 1);
    glVertex2f(x, y - height - 1);
    glVertex2f(x + width + 1, y);
    glVertex2f(x - width - 1, y);
    glEnd();
    //draw color cross
    glLineWidth(LineWidth);
    glBegin(GL_LINES);
    glColor4f(RGB[0], RGB[1], RGB[2], 1); //color, yes alpha
    glVertex2f(x, y + height);
    glVertex2f(x, y - height);
    glVertex2f(x + width, y);
    glVertex2f(x - width, y);
    glEnd();
}

//draw filled rectangle
void gl::DrawFilledBox(float x, float y, float width, float height, int* RGB)
{
    glBegin(GL_QUADS); //begin drawing filled outline rect, +1 pixel to each side to add a border
    glColor4f(0, 0, 0, 1); //black, yes alpha
    glVertex2f(x - (width / 2) - 1, y + 1); // bottom left
    glVertex2f(x - (width / 2) - 1, y - height - 1); //top left
    glVertex2f(x + (width / 2) + 1, y - height - 1); //top right
    glVertex2f(x + (width / 2) + 1, y + 1); //bottom right
    glEnd();
    glBegin(GL_QUADS);//begin drawing filled color rect
    glColor4f(RGB[0], RGB[1], RGB[2], 1);//color, yes alpha
    glVertex2f(x - (width / 2), y); // bottom left
    glVertex2f(x - (width / 2), y - height); //top left
    glVertex2f(x + (width / 2), y - height); //top right
    glVertex2f(x + (width / 2), y); //bottom right
    glEnd();
}

void gl::DrawCircle(float cx, float cy, float r, int num_segments, int* RGB) {
    glColor3ub(RGB[0], RGB[1], RGB[2]);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < num_segments; i++) {
        float theta = 2.0f * 3.1415926f * float(i) / float(num_segments);//get the current angle 
        float x = r * cosf(theta);//calculate the x component 
        float y = r * sinf(theta);//calculate the y component 
        glVertex2f(x + cx, y + cy);//output vertex 
    }
    glEnd();
}