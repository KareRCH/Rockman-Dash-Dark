#pragma once

#include "GameObjectComp.h"

BEGIN(Engine)

/// <summary>
/// 클라우드 스테이션을 사용하기 위해 필요한 컴포넌트.
/// 이 컴포넌트가 있어야 클라우스 스테이션에 접근할 수 있다.
/// 내부적으로 접근에 관련한 함수를 가지고 있으며, 
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