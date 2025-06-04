// UsingDynamicLoad.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <windows.h>
#include "../MyFuncLib/MyFuncLib.h"

int TestAdd(int a, int b)
{
	return a + b;
}

int main()
{
	int a = 10, b = 20 ,result =0;
	//////////////////////////////////////////////////////////////////////////
	// 함수 포인터 사용 예 	
	int (*pTestAddPtr)(int, int) = &TestAdd; //함수포인터 선언 및 초기화         
	result = pTestAddPtr(a, b);
	std::wcout << L"TestAdd(10, 20) = " << result << std::endl;
	//////////////////////////////////////////////////////////////////////////

	// DLL 로딩
#ifdef _DEBUG
	HMODULE hDll = LoadLibraryW(L"../x64/Debug/0_MyFuncLib.dll");
#else   
	HMODULE hDll = LoadLibraryW(L"../x64/Release/0_MyFuncLib.dll");
#endif	

	if (!hDll) {
		std::wcout << L"DLL 로딩 실패\n";
		return -1;
	}

	int (*pAdd)(int, int) = (int (*)(int, int))GetProcAddress(hDll, "MyFuncLib_Add");		
	if (!pAdd) {
		std::wcout << L"Add 함수 찾기 실패\n";
		FreeLibrary(hDll);
		return -2;
	}

	// 함수 호출
	result = pAdd(10, 20);
	std::wcout << L"MyFuncLib_Add(10, 20) = " << result << std::endl;

	// DLL 언로드
	FreeLibrary(hDll);
}
