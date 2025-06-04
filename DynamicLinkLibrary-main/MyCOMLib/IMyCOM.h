#pragma once
#include <Unknwn.h>

// {5EF1E288-F371-4AEB-8438-99B297314BA9}
static const CLSID CLSID_MyCOMObject =
{ 0x5ef1e288, 0xf371, 0x4aeb, { 0x84, 0x38, 0x99, 0xb2, 0x97, 0x31, 0x4b, 0xa9 } };

interface __declspec(uuid("AF2B7063-8666-418C-9E1B-47A64EF5A1AB")) 
IMyCOMObject : public IUnknown {  
    virtual void Hello() = 0;
	virtual HRESULT DoSomething(int value) = 0;
};  
const IID IID_IMyCOMObject = __uuidof(IMyCOMObject);

interface __declspec(uuid("1F47836D-6904-48DB-B87C-F67E3316D679"))
IMyCOMObject2 : public IMyCOMObject{
	virtual void Bye() = 0;
};
const IID IID_IMyCOMObject2 = __uuidof(IMyCOMObject2);

interface __declspec(uuid("D2ACA762-2529-4FC3-ABE1-51ECB35D775B"))
IMyCOMObjectExtraFeature : public IUnknown{
	virtual void Jump() = 0;
};
const IID IID_IMyCOMObjectExtraFeature = __uuidof(IMyCOMObjectExtraFeature);



interface __declspec(uuid("5A51E631-8D28-4F78-B3A6-DBD4B55E527E"))
IMyFactory : public IClassFactory
{
	STDMETHOD(CreateInstance)(IUnknown* pUnkOuter, REFIID riid, void** ppv)=0;
};
const IID IID_IMyFactory = __uuidof(IMyFactory);

/// <summary>
/// 
/// </summary>
interface __declspec(uuid("5B6CAE77-2D6A-4455-AF1E-6001D9591CA2"))
IOtherObject : public IUnknown{
	virtual void Say() = 0;
};
const IID IID_IOtherObject = __uuidof(IOtherObject);


interface __declspec(uuid("A1DCE58D-0266-4A0B-87BD-70266D8DD103"))
IMyFactory2 : public IMyFactory
{
	// 팩토리가 버전업을 해서 OtherObject도 생성할수 있다.
	virtual HRESULT CreateOther(IOtherObject** ppObj) = 0;
};
const IID IID_IMyFactory2 = __uuidof(IMyFactory2);


// Factory 생성 함수
extern "C" __declspec(dllexport) HRESULT CreateMyFactory(REFIID riid, void** ppv);




