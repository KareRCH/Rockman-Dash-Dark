#pragma once

#include "GameObjectComp.h"

#include "Utility/LogicDeviceBasic.h"
#include "BaseClass/CloudStation.h"
#include "Utility/ClassID.h"

BEGIN(Engine)

/// <summary>
/// Ŭ���� �����̼��� ����ϱ� ���� �ʿ��� ������Ʈ.
/// �� ������Ʈ�� �־�� Ŭ��콺 �����̼ǿ� ������ �� �ִ�.
/// ���������� ���ٿ� ������ �Լ��� ������ ������, 
/// </summary>
class ENGINE_DLL CCloudStationComp final : public CGameObjectComp
{
	DERIVED_CLASS(CGameObjectComp, CCloudStationComp)

public:
	static const _uint g_ClassID = ECast(EComponentID::CloudStation);

protected:
	explicit CCloudStationComp() = default;
	explicit CCloudStationComp(const CCloudStationComp& rhs);
	virtual ~CCloudStationComp() = default;

public:
	virtual HRESULT	Initialize_Prototype(void* Arg = nullptr) override;
	virtual HRESULT Initialize_Prototype(FSerialData& InputData);
	virtual HRESULT Initialize(void* Arg = nullptr) override;
	virtual HRESULT Initialize(FSerialData& InputData);
	virtual void	Priority_Tick(const _float& fTimeDelta) override;
	virtual void	Tick(const _float& fTimeDelta) override;
	virtual void	Late_Tick(const _float& fTimeDelta) override;
	virtual HRESULT	Render() override;

public:
	// ������Ÿ�� ���ۿ� �Լ�
	virtual FSerialData SerializeData_Prototype();
	// Ŭ�δ� ���� �Լ�
	virtual FSerialData SerializeData();

public:
	static	CCloudStationComp* Create();
	static	CCloudStationComp* Create(FSerialData& InputData);
	virtual CComponent* Clone(void* Arg = nullptr) override;
	virtual CComponent* Clone(FSerialData& InputData);

protected:
	virtual void	Free() override;

public:
	// Ŭ���� �����̼��� ������ ���� �Լ�
	HRESULT Open_CloudStation_Session(const wstring& strName, CCloudStation* pCloudStation);
	// 
	HRESULT Connect_CloudStation(const wstring& strName);
	void	Disconnect_CloudStation(_uint iIndex);

	template <typename T, typename = enable_if<is_base_of<CCloudStation, T>::value>>
	T*		Get_CloudStation(_uint iIndex);
	template <typename T, typename = enable_if<is_base_of<CCloudStation, T>::value>>
	T*		Get_LastCloudStation();

private:
	_bool Exists_CloudStation(CCloudStation* pCloudStation);

private:
	class CCloudStationMgr*		m_pCloudStationMgr = { nullptr };

	_uint						m_iNumCloudStations = { 0 };
	vector<CCloudStation*>		m_CloudStations;				// ����� Ŭ���� �����̼ǵ�
	map<const wstring, FGauge>	m_ReservedStations;				// ����� Ŭ���� �����̼ǰ� ��� �ִ� �ð�

	FGauge						m_fReconnectTime = FGauge(1.f);			// �翬�� ���ð�


};

template <typename T, typename>
T* CCloudStationComp::Get_CloudStation(_uint iIndex)
{
	if (iIndex < 0 || iIndex >= m_iNumCloudStations)
		return nullptr;

	return DynCast<T*>(m_CloudStations[iIndex]);
}

template<typename T, typename>
inline T* CCloudStationComp::Get_LastCloudStation()
{
	return DynCast<T*>(m_CloudStations.back());
}

template <>
struct TComponentTrait<CCloudStationComp::g_ClassID>
{
	using Class = CCloudStationComp;
};

END