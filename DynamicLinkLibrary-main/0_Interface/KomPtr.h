#pragma once
template<typename T>
class KomPtr
{
public: // ������ private��
	T* m_ptr;
public:
	// 1) �⺻ ������: null �����ͷ� �ʱ�ȭ
	KomPtr() noexcept
		: m_ptr(nullptr)
	{
	}

	// 2) raw �����ͷ� ������: AddRef ȣ��
	explicit KomPtr(T* ptr) noexcept
		: m_ptr(ptr)
	{
		if (m_ptr)
			m_ptr->AddRef();
	}

	// 3) ���� ������: �ٸ� ComPtr�� ����Ű�� �����͸� �����ͼ� AddRef
	KomPtr(const KomPtr& other) noexcept
		: m_ptr(other.m_ptr)
	{
		if (m_ptr)
			m_ptr->AddRef();
	}

	// 4) �̵� ������: other���� �����͸� ��������, other�� null��
	KomPtr(KomPtr&& other) noexcept
		: m_ptr(other.m_ptr)
	{
		other.m_ptr = nullptr;
	}

	// 5) �Ҹ���: �����Ͱ� ���� ������ Release ȣ��
	~KomPtr()
	{
		if (m_ptr)
			m_ptr->Release();
	}

	// 6) ���� ���� ������: ���� ������ Release �� �ٸ� ������ AddRef
	KomPtr& operator=(const KomPtr& other) noexcept
	{
		if (this != &other)
		{
			// ������ ������ �ִ� ������ ������ ����
			if (m_ptr)
				m_ptr->Release();

			// �� ������ �����ϰ� AddRef
			m_ptr = other.m_ptr;
			if (m_ptr)
				m_ptr->AddRef();
		}
		return *this;
	}

	// 7) �̵� ���� ������: ���� ������ Release �� other���� ������ �������� other null��
	KomPtr& operator=(KomPtr&& other) noexcept
	{
		if (this != &other)
		{
			if (m_ptr)
				m_ptr->Release();

			m_ptr = other.m_ptr;
			other.m_ptr = nullptr;
		}
		return *this;
	}

	// 8) ��� ���� ������: m_ptr->���
	T* operator->() const noexcept
	{
		return m_ptr;
	}

	// 9) ������ ������: *m_ptr
	T& operator*() const noexcept
	{
		return *m_ptr;
	}

	// 10) �ּ� ������ �����ε�: ȣ�� ������ &comPtr�� �ϸ� T**�� ��� ����
	//     (��: CreateInstance ���� �Լ� ȣ�� �� Out-Parameter�� ����)
	T** operator&() noexcept
	{
		return &m_ptr;
	}

	// 11) ���� ������ ��ȯ
	T* Get() const noexcept
	{
		return m_ptr;
	}

	// 12) ���� ������ Release �� null�� �ʱ�ȭ
	void Reset() noexcept
	{
		if (m_ptr)
		{
			m_ptr->Release();
			m_ptr = nullptr;
		}
	}

	// 13) �ܺο��� ������ raw �����͸� �����ϡ� ��: ���� ������ Release �� ptr �״�� ��������
	void Attach(T* ptr) noexcept
	{
		if (m_ptr)
			m_ptr->Release();
		m_ptr = ptr;
	}

	// 14) ���� �����͸� ��ȯ�ϰ�, ���δ� null�� ����� (Release ���� ����)
	T* Detach() noexcept
	{
		T* temp = m_ptr;
		m_ptr = nullptr;
		return temp;
	}

	// GetAddressOf(): m_ptr�� Release()�� ��, m_ptr�� �ּҸ� ��ȯ
	T** GetAddressOf() noexcept
	{
		// ������ ����Ű�� �ִ� ��ü�� ������ ����
		if (m_ptr)
		{
			m_ptr->Release();
			m_ptr = nullptr;
		}
		return &m_ptr;
	}

};


