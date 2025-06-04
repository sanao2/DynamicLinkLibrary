#include <iostream>
#include "KomPtr.h"
class IUnkown
{
public:
	virtual void AddRef() = 0;
	virtual void Release() = 0;
	virtual void QueryInterface(const char* name, void** ppOut) = 0;
};

class IParent : public IUnkown
{
public:
	virtual void Hello() = 0;
};
class Parent : public IParent
{
public:
	virtual ~Parent() {
		std::cout << __FUNCDNAME__ << "\n";
	}
	int ref = 1;
public:
	virtual void Hello() override {
		std::cout << __FUNCDNAME__ << "\n";
	};
	void AddRef() override
	{
		ref++;
	}
	void Release() override
	{
		ref--;
		if (ref == 0)
		{
			delete this;
		}
	}
	virtual void QueryInterface(const char* name, void** ppOut) override
	{

	}
};
class IChild: public IUnkown
{
public:
	virtual void Attack1() = 0;
};

class IChildOther : public IUnkown
{
public:
	virtual void Defence() = 0;
};

class Child : public Parent, public IChild,public IChildOther
{
public:
	~Child() override {
		std::cout << __FUNCDNAME__ << "\n";
	}
	void Attack1() override
	{
		std::cout << __FUNCDNAME__ << "\n";
	}
	void Defence() override
	{
		std::cout << __FUNCDNAME__ << "\n";
	}

	void AddRef() override
	{
		ref++;
	}
	void Release() override
	{
		ref--;
		if (ref == 0)
		{
			delete this;
		}
	}
	virtual void QueryInterface(const char* name, void** ppOut) override
	{
		if (strcmp(name, typeid(IChild).name())==0) // "class IChild" 
		{
			*ppOut = (IChild*)(this);
			AddRef();
			
		}
		else if (strcmp(name, typeid(IChildOther).name())==0) // "class IChildOther"
		{
			*ppOut = (IChildOther*)(this);
			AddRef();
		}
	}
};

Child* CreateChild()
{
	return new Child;
}

void CreateChild(Child** outChild)
{
	*outChild = new Child;	
}

void CreateChild(IChild** outIChild)
{
	*outIChild = new Child;
}


int main()
{		
	// 인터페이스 테스트
	Child* pChlid = CreateChild(); // 내부에서 생성 후 메모리 주소 리턴
	Parent* pParent = pChlid; // 부모타입으로 변환
	IChild* pInterface = pChlid;  // 부모타입으로 변환
	pParent->Hello();
	pInterface->Attack1();
	delete pChlid;
	
	// 이중 포인터 활용과 Release로 삭제하기
	pChlid = nullptr;
	pInterface = nullptr;
	CreateChild(&pChlid);	// 변수의 주소를 넘겨서 값을 받아오기
	pChlid->Attack1();
	pChlid->Release();
	
	
	// 클래스가 여러 인터페이스를 구현한 경우 다른 인터페이스 얻기
	pInterface = nullptr;
	CreateChild(&pInterface);
	IChildOther* pIChlidOther=nullptr;
	pInterface->QueryInterface("class IChildOther", (void**)&pIChlidOther);
	pInterface->Attack1();
	pIChlidOther->Defence();
	pInterface->Release();
	pIChlidOther->Release();


	// ComPtr테스트
	KomPtr<IChild> cptrIChild;
	CreateChild((IChild**)cptrIChild.GetAddressOf()); // &cptrIChild.m_ptr
	// cptrIChild 소멸자에서 Release() 를 호출해준다.
}