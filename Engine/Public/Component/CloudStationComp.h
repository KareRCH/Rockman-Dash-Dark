#pragma once

#include "GameObjectComp.h"

#include "Utility/LogicDeviceBasic.h"

BEGIN(Engine)

/// <summary>
/// Ŭ���� �����̼��� ����ϱ� ���� �ʿ��� ������Ʈ.
/// �� ������Ʈ�� �־�� Ŭ��콺 �����̼ǿ� ������ �� �ִ�.
/// ���������� ���ٿ� ������ �Լ��� ������ ������, 
/// </summary>
class CCloudStationComp final : public CGameObjectComp
{
	DERIVED_CLASS(CGameObjectComp, CCloudStationComp)
protected:
	explicit CCloudStationComp() = default;
	explicit CCloudStationComp(const CCloudStationComp& rhs);
	virtual ~CCloudStationComp() = default;

public:
	virtual HRESULT	Initialize_Prototype(void* Arg = nullptr) override;
	virtual HRESULT Initialize(void* Arg = nullptr) override;
	virtual void	Priority_Tick(const _float& fTimeDelta) override;
	virtual void	Tick(const _float& fTimeDelta) override;
	virtual void	Late_Tick(const _float& fTimeDelta) override;
	virtual HRESULT	Render() override;

public:
	static	CCloudStationComp* Create();
	virtual CComponent* Clone(void* Arg = nullptr) override;

protected:
	virtual void	Free() override;

public:
	// Ŭ���� �����̼��� ������ ���� �Լ�
	HRESULT Open_CloudStation_Session(const wstring& strName, class CCloudStationComp* pCloudStation);
	// 
	HRESULT Connect_CloudStation(const wstring& strName);
	void Disconnect_CloudStation(_uint iIndex);

private:
	_uint				m_iNumCloudStations = { 0 };
	vector<wstring>		m_CloudStationNames;	// ����� Ŭ���� �����̼ǵ�

	FGauge				m_fReconnectTime = FGauge(1.f);	// �翬�� ���ð�
	_float				m_fTimeout = { 10.f };


};

END