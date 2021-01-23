#pragma once
#include "includes.h"
#include "console.h"
#include "hook.h"
#include "mem.h"
#include "constants.h"
#include "esp.h"
#include "glDrawing.h"
#include "overlay.h"

#define show_console 1 //1 = show console ~ 0 = don't show console

#define PI 3.14159265358979323846 

using namespace std;

Entity* localPlayer = nullptr;
EntityList* entityList = nullptr;
float* viewMatrix = nullptr;
int playerCount = 0;
uintptr_t baseAddressMainMod = 0;
bool shutdown = false;


DWORD WINAPI MainThread(LPVOID param) {

	// calling GetModuleHandle with NULL returns .exe module
	baseAddressMainMod = (uintptr_t)GetModuleHandle(NULL);

	while (true) {

		if (!mem::IsBadReadPtr((Entity*)(baseAddressMainMod + localPlayerPointer))) {
			localPlayer = *(Entity**)(baseAddressMainMod + localPlayerPointer);
			entityList = *(EntityList**)(baseAddressMainMod + entityListPointer);
			viewMatrix = (float*)(baseAddressMainMod + viewMatrixPointer);
			playerCount = *(int*)(baseAddressMainMod + playerCountPointer);
		}
		
		if (GetAsyncKeyState(VK_NUMPAD5) & 1) { //if Insert is pressed make a short beep and free the console (if used)

			Entity* closestEntity = getClosestEnemyToCrosshair(localPlayer, entityList, isTeamGame(baseAddressMainMod), playerCount);

			Vector3 src = localPlayer->getPosition();
			Vector3 dst = closestEntity->getPosition();
			// correct aim down a bit
			dst.z -= 2;

			Vector2 angle;
			angle.x = (atan2f(dst.x - src.x, dst.y - src.y) * -1.0) / PI * 180.0;
			angle.y = asinf((dst.z - src.z) / src.Distance(dst)) * (180.0 / PI);

			float distance = getClosestEnemyToCrosshairFOVDistance(localPlayer, entityList, isTeamGame(baseAddressMainMod), playerCount);

			if (distance <= 13) {
				localPlayer->setViewAngleX(angle.x);
				localPlayer->setViewAngleY(angle.y);
				// short sleep to let the game time to snap on target
				Sleep(10);
				clickMouseLeft();
			}
		}

		if (GetAsyncKeyState(VK_NUMPAD6) & 1) { //if Insert is pressed make a short beep and free the console (if used)
			shootIfEnemyCloseToCrosshair(localPlayer, entityList, isTeamGame(baseAddressMainMod), playerCount);
		}

		//IMPORTANT: if you close the console manually the game/programm you injected this into will crash/close down!
		if (GetAsyncKeyState(VK_INSERT) & 1) { //if Insert is pressed make a short beep and free the console (if used)
			shutdown = true;
			Sleep(1000);
			if (show_console) {
				auto temp = GetConsoleWindow();
				FreeConsole();
				PostMessage(temp, WM_QUIT, 0, 0);
			}
			break;
		}
	}

	FreeLibraryAndExitThread((HMODULE)param, 0); //erease traces of your dll (!)
	return 0;
}



BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
) {
	switch (ul_reason_for_call) {
	case DLL_PROCESS_ATTACH: {
		DisableThreadLibraryCalls(hModule);
		CreateThread(nullptr, 0, MainThread, hModule, 0, nullptr);
		CreateThread(nullptr, 0, overlay::OverlayThread, hModule, 0, nullptr);
		if (show_console) console(hModule); //opens console if show_console is set to 1 (how? -> console.h)
	}

	case DLL_PROCESS_DETACH: {
		break;
	}
	}

	return TRUE;
}