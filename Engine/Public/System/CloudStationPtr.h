#pragma once

#include "Base.h"
#include "BaseClass/CloudStation.h"

BEGIN(Engine)

/// <summary>
/// 블랙보드를 참조하는 위크 포인터 개념의 객체
/// 따로 RefCount를 가지지 않는다.
/// </summary>
template <typename T,
	typename = enable_if_t<is_base_of_v<CCloudStation, T>>>
class FCloudStationPtr
{
public:
	explicit FCloudStationPtr()
		: m_pCloudStation(nullptr)
	{}
	explicit FCloudStationPtr(const FCloudStationPtr<T>& rhs) {}
	~FCloudStationPtr()
	{
		// 이미 만료된 것은 해제코드 작동을 안하니 중복 해제해도 문제 없음
		// RefCount 감소
		Safe_Release(m_pCloudStation);
	}

public:
	void Set_CloudStation(CCloudStation* const pBlackBoard)
	{
		if (!pBlackBoard)
			return;

		if (pBlackBoard->IsExpired())
			return;

		m_pCloudStation = pBlackBoard;
		// 설정된 블랙보드의 수명을 연장시킨다.
		pBlackBoard->AddRef();
	}

public:
	T* Get_CloudStation()
	{
		if (m_pCloudStation == nullptr)
			return nullptr;

		if (m_pCloudStation->IsExpired())
		{
			Safe_Release(m_pCloudStation);
			// 만료된 블랙보드는 자동으로 nullptr를 채워준다.
			return static_cast<T*>(m_pCloudStation = nullptr);
		}

		return static_cast<T*>(m_pCloudStation);
	}

public:
	_bool		IsAvailable() 
	{ 
		if (!m_pCloudStation)
			return nullptr;
		return  m_pCloudStation->IsExpired(); 
	}

private:
	CCloudStation* m_pCloudStation;
};

END