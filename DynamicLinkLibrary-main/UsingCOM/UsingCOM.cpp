// UsingCOM.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <windows.h>
#include "CoCreateInstanceExample.h"
#include "ManualFactoryExample.h"



int main()
{
    HRESULT hr = CoInitialize(nullptr);

    // 초기 전통적 COM객체를 사용하는 방법
    Run_CoCreateInstance_Example();  

    // 수동으로 팩토리를 사용하여 COM객체를 사용하는방법
    Run_ManualFactory_Example();  

    CoUninitialize();
}
