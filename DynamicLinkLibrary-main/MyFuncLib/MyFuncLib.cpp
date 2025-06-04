#include "pch.h"
#include "MyFuncLib.h"

int MyFuncLib_Add(int a, int b)
{
	OutputDebugStringW(L"__FUNCTION__");
	return a + b;
}

int MyFuncLib_Subtract(int a, int b)
{
	OutputDebugStringW(L"__FUNCTION__");
	return a - b;
}
