#pragma once

#include "Client_Define.h"
#include "Character_Common.h"


BEGIN(Engine)

END

BEGIN(Client)

/// <summary>
/// NPC 캐릭터 
/// </summary>
class CNpc_Common abstract : public CCharacter_Common
{
	DERIVED_CLASS(CCharacter_Common, CNpc_Common)

protected:
	explicit CNpc_Common();
	explicit CNpc_Common(const CNpc_Common& rhs);
	virtual ~CNpc_Common() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize_Prototype(FSerialData& InputData);
	virtual HRESULT Initialize(void* Arg = nullptr) override;
	virtual HRESULT Initialize(FSerialData& InputData);
	virtual void	Priority_Tick(const _float& fTimeDelta) override;
	virtual void	Tick(const _float& fTimeDelta) override;
	virtual void	Late_Tick(const _float& fTimeDelta) override;
	virtual HRESULT	Render() override;

public:
	virtual CGameObject* Clone(void* Arg = nullptr) PURE;

protected:
	virtual void			Free() override;

private:
	HRESULT			Initialize_Component();

public:		// 충돌 이벤트
	virtual void OnCollision(CGameObject* pDst, const FContact* pContact);
	virtual void OnCollisionEntered(CGameObject* pDst, const FContact* pContact);
	virtual void OnCollisionExited(CGameObject* pDst);
};

END