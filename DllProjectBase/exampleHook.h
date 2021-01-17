#include "includes.h"
// for hooking
void __declspec(naked)exampleAsmFunction() {
	__asm
	{
		mov eax, eax;
	}
}
