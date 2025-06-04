#pragma once
template<typename T>
class KomPtr
{
public: // 원래는 private임
	T* m_ptr;
public:
	// 1) 기본 생성자: null 포인터로 초기화
	KomPtr() noexcept
		: m_ptr(nullptr)
	{
	}

	// 2) raw 포인터로 생성자: AddRef 호출
	explicit KomPtr(T* ptr) noexcept
		: m_ptr(ptr)
	{
		if (m_ptr)
			m_ptr->AddRef();
	}

	// 3) 복사 생성자: 다른 ComPtr이 가리키는 포인터를 가져와서 AddRef
	KomPtr(const KomPtr& other) noexcept
		: m_ptr(other.m_ptr)
	{
		if (m_ptr)
			m_ptr->AddRef();
	}

	// 4) 이동 생성자: other에서 포인터만 가져오고, other는 null로
	KomPtr(KomPtr&& other) noexcept
		: m_ptr(other.m_ptr)
	{
		other.m_ptr = nullptr;
	}

	// 5) 소멸자: 포인터가 남아 있으면 Release 호출
	~KomPtr()
	{
		if (m_ptr)
			m_ptr->Release();
	}

	// 6) 복사 대입 연산자: 기존 포인터 Release → 다른 포인터 AddRef
	KomPtr& operator=(const KomPtr& other) noexcept
	{
		if (this != &other)
		{
			// 기존에 가지고 있던 포인터 있으면 해제
			if (m_ptr)
				m_ptr->Release();

			// 새 포인터 복사하고 AddRef
			m_ptr = other.m_ptr;
			if (m_ptr)
				m_ptr->AddRef();
		}
		return *this;
	}

	// 7) 이동 대입 연산자: 기존 포인터 Release → other에서 포인터 가져오고 other null로
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

	// 8) 멤버 접근 연산자: m_ptr->멤버
	T* operator->() const noexcept
	{
		return m_ptr;
	}

	// 9) 역참조 연산자: *m_ptr
	T& operator*() const noexcept
	{
		return *m_ptr;
	}

	// 10) 주소 연산자 오버로드: 호출 측에서 &comPtr을 하면 T**로 사용 가능
	//     (예: CreateInstance 같은 함수 호출 시 Out-Parameter로 전달)
	T** operator&() noexcept
	{
		return &m_ptr;
	}

	// 11) 내부 포인터 반환
	T* Get() const noexcept
	{
		return m_ptr;
	}

	// 12) 현재 포인터 Release 후 null로 초기화
	void Reset() noexcept
	{
		if (m_ptr)
		{
			m_ptr->Release();
			m_ptr = nullptr;
		}
	}

	// 13) 외부에서 생성된 raw 포인터를 “붙일” 때: 기존 포인터 Release 후 ptr 그대로 가져오기
	void Attach(T* ptr) noexcept
	{
		if (m_ptr)
			m_ptr->Release();
		m_ptr = ptr;
	}

	// 14) 내부 포인터를 반환하고, 내부는 null로 만든다 (Release 하지 않음)
	T* Detach() noexcept
	{
		T* temp = m_ptr;
		m_ptr = nullptr;
		return temp;
	}

	// GetAddressOf(): m_ptr을 Release()한 뒤, m_ptr의 주소를 반환
	T** GetAddressOf() noexcept
	{
		// 기존에 가리키고 있던 객체가 있으면 해제
		if (m_ptr)
		{
			m_ptr->Release();
			m_ptr = nullptr;
		}
		return &m_ptr;
	}

};


