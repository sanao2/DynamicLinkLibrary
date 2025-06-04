#include "ManualFactoryExample.h"


#include <windows.h>
#include "../MyCOMLib/IMyCOM.h"

#include <wrl.h>	// ComPtr
using namespace Microsoft::WRL;

#ifdef _DEBUG
#pragma comment(lib, "../x64/Debug/3_MyCOMLib.lib")
#else
#pragma comment(lib, "../x64/Release/3_MyCOMLib.lib")
#endif


void Run_ManualFactory_Example()
{	
	// 인터페이스 포인터를 다루는 스마트 포인터
	// 변수가 파괴될때 클래스 내부에서 인터페이스의 Release를 호출한다.
	IMyFactory2* pIMyFactory;
	ComPtr<IMyCOMObject2> cpMyCOMObject2;
	ComPtr<IMyCOMObjectExtraFeature> cpMyCOMObjectExtraFeature;
	ComPtr<IOtherObject> cpOtherObject;
	
	IID test = __uuidof(pIMyFactory);
	CreateMyFactory(test, // 팩토리의 인터페이스 ID
		(void**)&pIMyFactory); // 인터페이스 포인터를 저장할 변수
	
	// 팩토리와 1:1로 연결되는 COM객체
	pIMyFactory->CreateInstance(nullptr,
		__uuidof(cpMyCOMObject2), // COM객체를 형변환하여 사용할 인터페이스 ID
		(void**)cpMyCOMObject2.GetAddressOf()); // 인터페이스 포인터를 저장할 변수

	// COM객체의 사용
	cpMyCOMObject2->Hello();
	cpMyCOMObject2->Bye();

	// 팩토리가 부가적으로 생성한 COM객체
	pIMyFactory->CreateOther(cpOtherObject.GetAddressOf());
	// COM객체의 사용
	cpOtherObject->Say();

	// 같은 인스턴스에대한 다른 인터페이스 얻기
	// IMyCOMObjectExtraFeature
	cpMyCOMObject2->QueryInterface(__uuidof(cpMyCOMObjectExtraFeature),
		(void**)cpMyCOMObjectExtraFeature.GetAddressOf());
	cpMyCOMObjectExtraFeature->Jump();
	

	pIMyFactory->Release(); // 참조가 0일때 스스로 delete
}
