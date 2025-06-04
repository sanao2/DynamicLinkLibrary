// UsingDLL.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include "../MyFuncLib/MyFuncLib.h"

#ifdef _DEBUG
#pragma comment(lib, "../x64/Debug/0_MyFuncLib.lib")
#else   
#pragma comment(lib, "../x64/Release/0_MyFuncLib.lib")
#endif

int main()
{
	int a = 10, b = 20;
	int result = MyFuncLib_Add(a, b);


    std::cout << "Hello World!\n";
}
