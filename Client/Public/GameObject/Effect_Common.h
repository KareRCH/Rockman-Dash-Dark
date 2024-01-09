#pragma once

#include "Client_Define.h"
#include "BaseClass/GameObject.h"

#include "Component/PlaneModelComp.h"
#include "Utility/LogicDeviceBasic.h"

BEGIN(Client)

/// <summary>
/// 빌보드가 적용된 단순 애니메이션 재생 이펙트
/// 인스턴싱 버전이 아님
/// </summary>
class CEffect_Common : public CGameObject
{
	DERIVED_CLASS(CGameObject, CEffect_Common)

protected:
	explicit CEffect_Common();
	explicit CEffect_Common(const CEffect_Common& rhs);
	virtual ~CEffect_Common() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* Arg = nullptr) override;
	virtual void	Priority_Tick(const _float& fTimeDelta) override;
	virtual void	Tick(const _float& fTimeDelta) override;
	virtual void	Late_Tick(const _float& fTimeDelta) override;
	virtual HRESULT	Render() override;

public:
	static CEffect_Common*	Create();
	CGameObject*			Clone(void* Arg = nullptr);

protected:
	virtual void			Free() override;

public:
	virtual FSerialData SerializeData() override;

public:
	HRESULT Initialize_Component();

private:
	class CPlaneModelComp* m_pPlaneModelComp = { nullptr };

	_float			m_fAnimSpeed = { 0.f };
	FGauge			m_fAnimTime = FGauge(0.f);
	FGauge			m_fLifeTime = FGauge(1.f);

};

END