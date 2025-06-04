#include "pch.h"
#include "StaticImportDLL.h"

int StaticImport_Add(int a, int b)
{
	OutputDebugStringW(L"__FUNCTION__");
	return a + b;
}

int StaticImport_Subtract(int a, int b)
{
	OutputDebugStringW(L"__FUNCTION__");
	return a - b;
}
