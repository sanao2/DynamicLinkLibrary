#pragma once
#include "IMyCOM.h"

class MyCOMObject : 
	public IMyCOMObject2,public IMyCOMObjectExtraFeature
{
	ULONG m_refCount = 1;
public:
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppv) override 
	{
		if (riid == IID_IUnknown || riid == IID_IMyCOMObject) 
			*ppv = static_cast<IMyCOMObject*>(this);
		else if ( riid == IID_IMyCOMObject2) 
			*ppv = static_cast<IMyCOMObject2*>(this);
		else if (riid == IID_IMyCOMObjectExtraFeature)
			*ppv = static_cast<IMyCOMObjectExtraFeature*>(this);
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

	// IMyCOMObject
	void Hello() override {
		std::cout << "Hello from MyCOMObject!" << std::endl;
	}

	// 함수 SUCESS결과  S_OK,S_FALSE
	// 함수 ERROR결과 E_.... 
	HRESULT DoSomething(int value) override
	{
		switch (value)
		{
		case 1:
			return E_INVALIDARG;
		case 2:
			return E_FAIL;
		case 3:
			return E_OUTOFMEMORY;		
		default:
			return S_FALSE;
		}
		return S_OK;
	}

	// IMyCOMObject2
	void Bye() override {
		std::cout << "Bye from MyCOMObject!" << std::endl;
	}

	// IMyCOMObjectExtraFeature
	void Jump() override {
		std::cout << "Jump from MyCOMObject!" << std::endl;
	}
};
