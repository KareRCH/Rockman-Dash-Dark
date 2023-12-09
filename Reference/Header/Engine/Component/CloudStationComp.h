#pragma once

#include "GameObjectComp.h"

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


private:
	


};

END