#pragma once

#include "Base.h"
#include "BaseClass/CloudStation.h"

BEGIN(Engine)

/// <summary>
/// �����带 �����ϴ� ��ũ ������ ������ ��ü
/// ���� RefCount�� ������ �ʴ´�.
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
		// �̹� ����� ���� �����ڵ� �۵��� ���ϴ� �ߺ� �����ص� ���� ����
		// RefCount ����
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
		// ������ �������� ������ �����Ų��.
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
			// ����� ������� �ڵ����� nullptr�� ä���ش�.
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