#pragma once

#include "GameObjectComp.h"

#include "Utility/LogicDeviceBasic.h"
#include "BaseClass/CloudStation.h"
#include "Utility/ClassID.h"

BEGIN(Engine)

/// <summary>
/// 클라우드 스테이션을 사용하기 위해 필요한 컴포넌트.
/// 이 컴포넌트가 있어야 클라우스 스테이션에 접근할 수 있다.
/// 내부적으로 접근에 관련한 함수를 가지고 있으며, 
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
	// 프로토타입 제작용 함수
	virtual FSerialData SerializeData_Prototype();
	// 클로닝 전용 함수
	virtual FSerialData SerializeData();

public:
	static	CCloudStationComp* Create();
	static	CCloudStationComp* Create(FSerialData& InputData);
	virtual CComponent* Clone(void* Arg = nullptr) override;
	virtual CComponent* Clone(FSerialData& InputData);

protected:
	virtual void	Free() override;

public:
	// 클라우드 스테이션의 세션을 여는 함수
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
	vector<CCloudStation*>		m_CloudStations;				// 연결된 클라우드 스테이션들
	map<const wstring, FGauge>	m_ReservedStations;				// 예약된 클라우드 스테이션과 대기 최대 시간

	FGauge						m_fReconnectTime = FGauge(1.f);			// 재연결 대기시간


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