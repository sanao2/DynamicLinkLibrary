// dllmain.cpp : DLL 애플리케이션의 진입점을 정의합니다.
#include "pch.h"
#include "IMyCOM.h"
#include "MyFactory.h"

static HMODULE g_hModule = nullptr;

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		g_hModule = hModule;
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

std::wstring GuidToString(REFCLSID clsid)
{
	LPOLESTR str = nullptr;
	if (SUCCEEDED(StringFromCLSID(clsid, &str)))
	{
		std::wstring result(str);
		CoTaskMemFree(str);
		return result;
	}
	return L"";
}

void OutputLastErrorMessage(DWORD errorCode, std::wstring& out)
{
	wchar_t* messageBuffer = nullptr;
	FormatMessageW(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, errorCode, 0, (LPWSTR)&messageBuffer, 0, NULL);

	if (messageBuffer)
	{
		out = messageBuffer;
		LocalFree(messageBuffer);
	}
	else
	{
		out = L"Unknown error";
	}
}

// __declspec(dllexport) 를 붙여야 COM 인터페이스를 외부에서 사용할 수 있도록 함
// 링커 -> 입력 -> 모듈정의파일에 MyCOMLib.def 파일을 추가해야 함
// COM 런타임에서 DLL 등록할 때 호출됨
extern "C" HRESULT __stdcall DllRegisterServer()
{
	OutputDebugString(L"DllRegisterServer called\n");
	wchar_t dllPath[MAX_PATH];
	DWORD error = GetModuleFileNameW(g_hModule, dllPath, MAX_PATH);
	if (error == 0)
	{
		std::wstring msg;
		OutputLastErrorMessage(GetLastError(), msg);
		MessageBoxW(nullptr, msg.c_str(), L"GetModuleFileNameW()", MB_OK | MB_ICONERROR);
		return SELFREG_E_CLASS;
	}
	std::wstring keyPath = L"CLSID\\" + GuidToString(CLSID_MyCOMObject) + L"\\InprocServer32";

	HKEY hKey;
	LSTATUS status = RegCreateKeyExW(HKEY_CLASSES_ROOT, keyPath.c_str(), 0, NULL, 0, KEY_WRITE, NULL, &hKey, NULL);
	if (status != ERROR_SUCCESS)
	{
		std::wstring msg;
		OutputLastErrorMessage(status, msg);
		MessageBoxW(nullptr, msg.c_str(), L"RegCreateKeyExW()", MB_OK | MB_ICONERROR);
		return SELFREG_E_CLASS;
	}

	// DLL 경로 등록
	RegSetValueExW(hKey, NULL, 0, REG_SZ, (BYTE*)dllPath, (lstrlenW(dllPath) + 1) * sizeof(wchar_t));

	// Threading model도 함께 등록 (필수는 아님)
	const wchar_t* model = L"Apartment";
	RegSetValueExW(hKey, L"ThreadingModel", 0, REG_SZ, (BYTE*)model, (lstrlenW(model) + 1) * sizeof(wchar_t));

	RegCloseKey(hKey);
	return S_OK;
}

// DllUnregisterServer 구현
extern "C" HRESULT __stdcall DllUnregisterServer()
{
	std::wstring clsidPath = L"CLSID\\" + GuidToString(CLSID_MyCOMObject);

	// InprocServer32 키까지 제거
	LSTATUS status = RegDeleteKeyW(HKEY_CLASSES_ROOT, clsidPath.c_str());
	if (status != ERROR_SUCCESS)
	{
		std::wstring msg;
		OutputLastErrorMessage(status, msg);
		MessageBoxW(nullptr, msg.c_str(), L"RegDeleteKeyW()", MB_OK | MB_ICONERROR);
		return SELFREG_E_CLASS;
	}

	return S_OK;
}


// COM 런타임에서 클래스 요청할 때 호출됨
extern "C" HRESULT __stdcall DllGetClassObject(REFCLSID clsid, REFIID iid, void** ppv)
{
	OutputDebugStringW(L"DllGetClassObject called\n");

	if (ppv == nullptr)
		return E_POINTER;

	*ppv = nullptr;

	if (clsid != CLSID_MyCOMObject)
		return CLASS_E_CLASSNOTAVAILABLE;

	MyFactory* factory = new (std::nothrow) MyFactory();
	if (!factory)
		return E_OUTOFMEMORY;

	HRESULT hr = factory->QueryInterface(iid, ppv);
	factory->Release();  // QueryInterface 성공 시 내부 AddRef 증가, 실패 시 delete됨

	return hr;
}