#pragma once
#include "includes.h"

namespace mem
{
	void Patch(BYTE* dst, BYTE* src, unsigned int size);
	bool IsBadReadPtr(void* p);
	void Nop(BYTE* dst, unsigned int size);
	char* ScanBasic(char* pattern, char* mask, char* begin, intptr_t size);
	char* ScanInternal(char* pattern, char* mask, char* begin, intptr_t size);
	char* ScanModIn(char* pattern, char* mask, std::string modName);
	std::vector<BYTE> NopBytesR(void* address, int amountOfBytesToNop);
	void WriteBytes(BYTE* dst, BYTE* src, unsigned int size);
	void WriteBytes(void* address, std::vector<BYTE>bytesToRestore);
	uintptr_t ResolveMLPointer(uintptr_t ptr, std::vector<unsigned int> offsets);
}