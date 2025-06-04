#pragma once
#include "IMyCOM.h"

class MyFactory : public IMyFactory2 {
	ULONG m_refCount = 1;
public:
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppv) override 
	{
		if (riid == IID_IUnknown || riid == IID_IClassFactory) 
			*ppv = static_cast<IClassFactory*>(this);
		else if (riid == IID_IMyFactory)
			*ppv = static_cast<IMyFactory*>(this);
		else if (riid == IID_IMyFactory2)
			*ppv = static_cast<IMyFactory2*>(this);
		else {
			*ppv = nullptr;
			return E_NOINTERFACE;
		}
		AddRef();
		return S_OK;
	}
	ULONG STDMETHODCALLTYPE AddRef() override { return ++m_refCount; }
	ULONG STDMETHODCALLTYPE Release() override {
		if (--m_refCount == 0) { delete this; return 0; }
		return m_refCount;
	}
	
	// 전통적인 COM프로그래밍은 팩토리는 하나의 클래스 인스턴스만 생성한다.
	STDMETHOD(CreateInstance)(IUnknown* pUnkOuter, REFIID riid, void** ppv) override;
	STDMETHOD(LockServer)(BOOL) { return S_OK; }


	HRESULT CreateOther(IOtherObject** ppObj) override;
};

extern "C" HRESULT CreateMyFactory(REFIID riid, void** ppv);
