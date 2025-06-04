#pragma once
#include "IMyCOM.h"


class OtherObject : public IOtherObject {
	ULONG m_refCount = 1;
public:
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppv) override {
		if (riid == IID_IUnknown || riid == IID_IOtherObject) {
			*ppv = static_cast<IOtherObject*>(this);
			AddRef();
			return S_OK;
		}
		*ppv = nullptr;
		return E_NOINTERFACE;
	}
	ULONG STDMETHODCALLTYPE AddRef() override { return ++m_refCount; }
	ULONG STDMETHODCALLTYPE Release() override {
		if (--m_refCount == 0) { delete this; return 0; }
		return m_refCount;
	}
	void Say() override {
		std::cout << "Say from MyCOMObject!" << std::endl;
	}
};
