#pragma once

#include "Client_Define.h"
#include "Character_Common.h"
#include "Utility/LogicDeviceBasic.h"

BEGIN(Engine)
class CCommonModelComp;
END



BEGIN(Client)


/// <summary>
/// 일반적으로 쓰이는 대미지용 콜리전
/// </summary>
class CDamageCollision : public CCharacter_Common
{
	DERIVED_CLASS(CCharacter_Common, CDamageCollision)

protected:
	explicit CDamageCollision();
	explicit CDamageCollision(const CDamageCollision& rhs);
	virtual ~CDamageCollision() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize_Prototype(const _float3 vPos);
	virtual HRESULT Initialize(void* Arg = nullptr) override;
	virtual HRESULT Initialize(const _float3 vPos);
	virtual void	Priority_Tick(const _float& fTimeDelta) override;
	virtual void	Tick(const _float& fTimeDelta) override;
	virtual void	Late_Tick(const _float& fTimeDelta) override;
	virtual HRESULT	Render() override;

public:
	static CDamageCollision* Create();
	static CDamageCollision* Create(const _float3 vPos);
	CGameObject* Clone(void* Arg = nullptr);

protected:
	virtual void	Free() override;

public:
	virtual FSerialData SerializeData() override;

private:
	HRESULT	Initialize_Component();

public:		// 충돌 이벤트
	virtual void OnCollision(CGameObject* pDst, const FContact* pContact);
	virtual void OnCollisionEntered(CGameObject* pDst, const FContact* pContact);
	virtual void OnCollisionExited(CGameObject* pDst);

private:
	CCommonModelComp* m_pModelComp = { nullptr };

public:
	void Set_LifeTime(_float Value) { m_fLifeTime = FGauge(Value); }

private:
	FGauge		m_fLifeTime = FGauge(0.1f);
};

END