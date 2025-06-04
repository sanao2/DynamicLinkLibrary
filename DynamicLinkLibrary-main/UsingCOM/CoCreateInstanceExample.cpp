#include "CoCreateInstanceExample.h"

#include "../MyCOMLib/IMyCOM.h"
#include <windows.h>

void Run_CoCreateInstance_Example()
{
	IMyCOMObject2* p = nullptr;
	// COM 라이브러리를 사용하기위해 필요한것 
	// 1. 레지스트리에 등록된 ClassID (DLL위치가 저장됨) 
	// 2. 라이브러리 클래스의 인스턴스를 사용하기위한 인터페이스
	HRESULT hr = CoCreateInstance(CLSID_MyCOMObject, // 생성할 COM 클래스 ID 
		nullptr, CLSCTX_INPROC_SERVER, // 내부 프로세스에 COM객체 인스턴스를 생성하겠다.
		__uuidof(p),   // 인터페이스 ID
		(void**)&p);  // 생성된 인스턴스의 인터페이스 주소를 저장한 변수 
	
	if (SUCCEEDED(hr)) 
	{
		// Client(호출하는쪽) / Server(COM객체 인스턴스)    
		p->Hello();
		p->Bye();

		// 생성한 인스턴스 해제
		p->Release(); 	
	}
	
}
