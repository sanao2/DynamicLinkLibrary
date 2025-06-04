#include "pch.h"
#include "MyFactory.h"
#include "MyCOMObject.h"
#include "OtherObject.h"

STDMETHODIMP MyFactory::CreateInstance(IUnknown* pUnkOuter, REFIID riid, void** ppv)
{
	if (ppv == nullptr)
		return E_POINTER;
	*ppv = nullptr;

	if (pUnkOuter != nullptr)
		return CLASS_E_NOAGGREGATION; // 이 예제는 Aggregation을 지원하지 않음

	MyCOMObject* pObj = new (std::nothrow) MyCOMObject();
	if (!pObj)
		return E_OUTOFMEMORY;

	HRESULT hr = pObj->QueryInterface(riid, ppv);
	pObj->Release(); // 성공 시 내부 refcount 2 → 여기서 1 줄임
	return hr;
}

HRESULT MyFactory::CreateOther(IOtherObject** ppObj)
{
	if (!ppObj) return E_POINTER;
	*ppObj = new OtherObject();
	return S_OK;
}

extern "C" HRESULT CreateMyFactory(REFIID riid, void** ppv)
{
	if (!ppv)
		return E_POINTER;

	*ppv = nullptr;

	MyFactory* pObj = new (std::nothrow) MyFactory();
	if (!pObj)
		return E_OUTOFMEMORY;

	HRESULT hr = pObj->QueryInterface(riid, ppv);
	pObj->Release();  // QueryInterface가 AddRef 했으므로 여기서 1 줄임

	return hr;
}
