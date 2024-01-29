#pragma once

#include "Client_Define.h"
#include "Character_Common.h"
#include "Utility/LogicDeviceBasic.h"

BEGIN(Engine)
class CCommonModelComp;
END




BEGIN(Client)

/// <summary>
/// 레이저
/// </summary>
class CWeapon_Laser : public CCharacter_Common
{
	DERIVED_CLASS(CCharacter_Common, CWeapon_Laser)

protected:
	explicit CWeapon_Laser();
	explicit CWeapon_Laser(const CWeapon_Laser& rhs);
	virtual ~CWeapon_Laser() = default;

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
	static CWeapon_Laser* Create();
	static CWeapon_Laser* Create(const _float3 vPos);
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

private:
	_float		m_fRange = { 30.f };
	_float3		m_vContactPoint = {};
	_bool		m_bIsContact = { false };
};

END