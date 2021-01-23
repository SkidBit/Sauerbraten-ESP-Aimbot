#include "overlay.h"

void overlay::ResizeWindow()
{
	//get physical window bounds
	GetWindowRect(GameHWND, &WindowRect);
	//get logical client bounds
	GetClientRect(GameHWND, &ClientRect);
	//width and height of client rect
	SWidth = ClientRect.right - ClientRect.left;
	SHeight = ClientRect.bottom - ClientRect.top;
	int border = (WindowRect.right - WindowRect.left - SWidth) / 2; //windowed mode border
	//physical screen x,y. logical width and height ie 1920x1080
	SetWindowPos(EspHWND, 0, WindowRect.left + border, WindowRect.bottom - SHeight - border, SWidth, SHeight, 0);
	//physical screen x,y. logical width and length ie 1920x1080
	glViewport(0, 0, SWidth, SHeight); //this was wrong
	//all logical
	glOrtho(ClientRect.left, ClientRect.right, ClientRect.bottom, ClientRect.top, 0, 1);
}

void overlay::Initiate()
{
	GameHWND = NULL;
	while (!GameHWND)
	{
		GameHWND = FindWindow(0, "Cube 2: Sauerbraten");
		Sleep(100);
	}

	CurrentInstance = GetModuleHandle(NULL);
	overlay::ResizeWindow();
}

//setup openGL for drawing
void overlay::SetupGL()
{
	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),
		1,                                  // Version Number
		PFD_DRAW_TO_WINDOW |              // Format Must Support Window
		PFD_SUPPORT_OPENGL |              // Format Must Support OpenGL
//        PFD_SUPPORT_GDI|                  // Format Must Support GDI
		PFD_SUPPORT_COMPOSITION |         // Format Must Support Composition
		PFD_DOUBLEBUFFER,                 // Must Support Double Buffering
		PFD_TYPE_RGBA,                    // Request An RGBA Format
		32,                               // Select Our Color Depth
		0, 0, 0, 0, 0, 0,                 // Color Bits Ignored
		8,                                // An Alpha Buffer
		0,                                // Shift Bit Ignored
		0,                                // No Accumulation Buffer
		0, 0, 0, 0,                       // Accumulation Bits Ignored
		0 ,                               // No Z-Buffer (Depth Buffer)
		8,                                // Some Stencil Buffer
		0,                                // No Auxiliary Buffer
		PFD_MAIN_PLANE,                   // Main Drawing Layer
		0,                                // Reserved
		0, 0, 0                           // Layer Masks Ignored
	};
	int PixelFormat = ChoosePixelFormat(MainHDC, &pfd);
	if (!PixelFormat)
	{
		MessageBox(0, "Failed to get pixel format!", "Error", MB_ICONERROR | MB_OK);
		ExitThread(0);
	}
	if (!SetPixelFormat(MainHDC, PixelFormat, &pfd))
	{
		MessageBox(0, "Failed to set pixel format!", "Error", MB_ICONERROR | MB_OK);
		ExitThread(0);
	}
	if (!DescribePixelFormat(MainHDC, PixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd))
	{
		MessageBox(0, "Failed to describe pixel format!", "Error", MB_ICONERROR | MB_OK);
		ExitThread(0);
	}
	MainHGLRC = wglCreateContext(MainHDC);
	if (!MainHGLRC)
	{
		MessageBox(0, "Failed to create rendering context!", "Error", MB_ICONERROR | MB_OK);
		ExitThread(0);
	}
	if (!wglMakeCurrent(MainHDC, MainHGLRC))
	{
		MessageBox(0, "Failed to set rendering context!", "Error", MB_ICONERROR | MB_OK);
		ExitThread(0);
	}
	//glEnable(GL_ALPHA_TEST);
	//glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(GL_BLEND);
	glMatrixMode(GL_PROJECTION);
	ResizeWindow();
	//color to set when screen is cleared, black with no alpha
	glClearColor(0.f, 0.f, 0.f, 0.f);
}

//window message handling
LRESULT APIENTRY overlay::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_CREATE:
	{
		MainHDC = GetDC(hWnd);
		SetupGL();
		return 0;
	}
	case WM_CLOSE:
	{
		DestroyWindow(hWnd);
	}
	//release resources
	case WM_DESTROY:
	{
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(MainHGLRC);
		DeleteDC(MainHDC);
		ReleaseDC(hWnd, MainHDC);
		PostQuitMessage(0);
		UnregisterClass(WName, CurrentInstance);
	}
	//our paint handler, invoked by invalidaterect()
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		BeginPaint(hWnd, &ps);
		//redraw the background
		glClear(GL_COLOR_BUFFER_BIT);

		if (isTeamGame(baseAddressMainMod)) {
			for (int i = 1; i < playerCount + 1; i++) {
				if (entityList->entities[i] != nullptr
					&& isEntityValid(entityList->entities[i])
					&& strcmp(localPlayer->getTeam(), entityList->entities[i]->getTeam()) != 0) {

					Vector2 screenCoords;
					Vector3 entityPos = entityList->entities[i]->getPosition();
					entityPos.z -= 15;

					if (WorldToScreen(entityPos, screenCoords, viewMatrix, SWidth, SHeight))
					{
						//draw snaplines to entity
						gl::DrawSnaplines(screenCoords.x, screenCoords.y, SWidth, SHeight, colorRed);
					}
				}
			}
		}
		else {
			for (int i = 1; i < playerCount + 1; i++) {
				if (entityList != nullptr
					&& entityList->entities[i] != nullptr
					&& isEntityValid(entityList->entities[i])
					&& strcmp(localPlayer->getName(), entityList->entities[i]->getName()) != 0) {

					Vector2 screenCoords;
					Vector3 entityPos = entityList->entities[i]->getPosition();
					entityPos.z -= 15;

					if (WorldToScreen(entityPos, screenCoords, viewMatrix, SWidth, SHeight))
					{
						//draw snaplines to entity
						gl::DrawSnaplines(screenCoords.x, screenCoords.y, SWidth, SHeight, colorRed);
					}
				}
			}
		}

		// draw box
		for (int i = 1; i < playerCount + 1; i++) {
			if (entityList != nullptr
				&& entityList->entities[i] != nullptr
				&& isEntityValid(entityList->entities[i])) {

				Vector2 screenCoords;
				Vector2 screenCoordsPlusHeight;
				Vector3 playerPos = entityList->entities[i]->getPosition();
				Vector3 playerPosPlusHeight = entityList->entities[i]->getPosition();
				playerPosPlusHeight.z -= 15;

				bool inFOV = WorldToScreen(playerPos, screenCoords, viewMatrix, SWidth, SHeight);
				WorldToScreen(playerPosPlusHeight, screenCoordsPlusHeight, viewMatrix, SWidth, SHeight);

				float height = screenCoords.y - screenCoordsPlusHeight.y;
				float width = height * (45.f / 80.f);

				if (inFOV)
				{
					gl::DrawBox(screenCoords.x, screenCoords.y, width, height, colorRed);
				}
			}
		}
		float distance = getClosestEnemyToCrosshairFOVDistance(localPlayer, entityList, isTeamGame(baseAddressMainMod), playerCount);

		if (distance <= 13) {
			gl::DrawCircle(SWidth / 2, SHeight / 2, 100, 20, colorRed);
		}
		else {
			gl::DrawCircle(SWidth / 2, SHeight / 2, 100, 20, colorGrey);
		}

		//copy the backbuffer into the window
		SwapBuffers(MainHDC);
		//end painting
		EndPaint(hWnd, &ps);
		return 0;
	}
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

//constantly redraw screen
DWORD WINAPI overlay::RedrawLoop(LPVOID PARAMS)
{
	while (true && !shutdown)
	{
		overlay::ResizeWindow();
		InvalidateRect(EspHWND, NULL, false);
	}
	ExitThread(0);
}

DWORD WINAPI overlay::OverlayThread(LPVOID param) {
	Initiate();
	WNDCLASSEX WClass;
	MSG Msg;
	WClass.cbSize = sizeof(WNDCLASSEX);
	WClass.style = 0;
	WClass.lpfnWndProc = WndProc;
	WClass.cbClsExtra = 0;
	WClass.cbWndExtra = 0;
	WClass.hInstance = CurrentInstance;
	WClass.hIcon = NULL;
	WClass.hCursor = NULL;
	WClass.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
	WClass.lpszMenuName = NULL;
	WClass.lpszClassName = WName;
	WClass.hIconSm = NULL;
	if (!RegisterClassEx(&WClass))
	{
		MessageBox(0, "Window Registration Failed! Is another window already open? If not, kill the process and try again.", "Error!", MB_OK);
		ExitThread(0);
	}
	EspHWND = CreateWindowEx(
		WS_EX_TRANSPARENT | WS_EX_TOPMOST | WS_EX_LAYERED,
		WName,
		WName,
		WS_POPUP | WS_VISIBLE | WS_MAXIMIZE,
		WindowRect.left,
		WindowRect.top - 1,
		SWidth,
		SHeight + 1,
		NULL, NULL, CurrentInstance, NULL);
	if (EspHWND == NULL)
	{
		MessageBox(0, "Window Creation Failed!", "Error!", MB_OK);
		UnregisterClass(WName, CurrentInstance);
		ExitThread(0);
	}

	//make sure our window is layered
		//NOTE: SOME WINDOWS VERSIONS DO NOT SUPPORT THIS
	if (!SetLayeredWindowAttributes(EspHWND, RGB(0, 0, 0), 255, LWA_COLORKEY | LWA_ALPHA))
	{
		MessageBox(0, "Failed to set layered window attributes!", "Error", MB_ICONERROR | MB_OK);
		UnregisterClass(WName, CurrentInstance);
		ExitThread(0);
	}

	BOOL IsEnabled;
	if (DwmIsCompositionEnabled(&IsEnabled) != S_OK)
	{
		MessageBox(0, "Failed to check if DWM window composition is enabled! Assuming false!", "Error", MB_ICONERROR | MB_OK);
		UnregisterClass(WName, CurrentInstance);
		ExitThread(0);
	}

	if (!IsEnabled)
	{
		MessageBox(0, "DWM window composition is not enabled!", "Error", MB_ICONERROR | MB_OK);
		UnregisterClass(WName, CurrentInstance);
		ExitThread(0);
	}

	//the region that is not blurred is rendered based off the alpha channel, therefore, we create an invalid region so that nothing is blurred but the alpha blending remains. We need this due to... who the fuck knows why layered window attributes wont work? seriously tell me please :'(
	DWM_BLURBEHIND bb = { DWM_BB_ENABLE | DWM_BB_BLURREGION, true, CreateRectRgn(0, 0, -1, -1), true };
	//enable trasnparency via dwm
	//NOTE: SOME WINDOWS VERSIONS DO NOT SUPPORT THIS
	if (DwmEnableBlurBehindWindow(EspHWND, &bb) != S_OK)
	{
		MessageBox(0, "Failed to enable DWM window blur!", "Error", MB_ICONERROR | MB_OK);
		UnregisterClass(WName, CurrentInstance);
		ExitThread(0);
	}

	ShowWindow(EspHWND, 1);

	CreateThread(0, 0, &RedrawLoop, 0, 0, 0);

	//message loop break on shutdown here
	while (GetMessage(&Msg, NULL, 0, 0) > 0 && !shutdown)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	ExitThread(0);
}