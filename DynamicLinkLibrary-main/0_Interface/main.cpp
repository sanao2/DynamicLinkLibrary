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
	// �������̽� �׽�Ʈ
	Child* pChlid = CreateChild(); // ���ο��� ���� �� �޸� �ּ� ����
	Parent* pParent = pChlid; // �θ�Ÿ������ ��ȯ
	IChild* pInterface = pChlid;  // �θ�Ÿ������ ��ȯ
	pParent->Hello();
	pInterface->Attack1();
	delete pChlid;
	
	// ���� ������ Ȱ��� Release�� �����ϱ�
	pChlid = nullptr;
	pInterface = nullptr;
	CreateChild(&pChlid);	// ������ �ּҸ� �Ѱܼ� ���� �޾ƿ���
	pChlid->Attack1();
	pChlid->Release();
	
	
	// Ŭ������ ���� �������̽��� ������ ��� �ٸ� �������̽� ���
	pInterface = nullptr;
	CreateChild(&pInterface);
	IChildOther* pIChlidOther=nullptr;
	pInterface->QueryInterface("class IChildOther", (void**)&pIChlidOther);
	pInterface->Attack1();
	pIChlidOther->Defence();
	pInterface->Release();
	pIChlidOther->Release();


	// ComPtr�׽�Ʈ
	KomPtr<IChild> cptrIChild;
	CreateChild((IChild**)cptrIChild.GetAddressOf()); // &cptrIChild.m_ptr
	// cptrIChild �Ҹ��ڿ��� Release() �� ȣ�����ش�.
}