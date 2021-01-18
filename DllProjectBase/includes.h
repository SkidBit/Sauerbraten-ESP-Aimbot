#pragma once

#define WIN32_LEAN_AND_MEAN // Selten verwendete Komponenten aus Windows-Headern ausschlieﬂen

#include <windows.h>
#include <iostream>
#include <stdio.h>
#include "ntdll.h"
#include <vector>
#include <algorithm>
#include "string.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include <dwmapi.h>
#include <TlHelp32.h>
#pragma comment( lib, "OpenGL32.lib" )
#pragma comment (lib, "glu32.lib")
#pragma comment(lib, "Dwmapi.lib")
#include "math.h"