#pragma once

#include "Client_Define.h"
#include "GameObject/Character_Common.h"


BEGIN(Engine)

class CCommonModelComp;

END


BEGIN(Client)

/// <summary>
/// 잠자리형 리버봇
/// </summary>
class CReaverBot_Balfura : public CCharacter_Common
{
	DERIVED_CLASS(CCharacter_Common, CReaverBot_Balfura)

protected:
	explicit CReaverBot_Balfura();
	explicit CReaverBot_Balfura(const CReaverBot_Balfura& rhs);
	virtual ~CReaverBot_Balfura() = default;

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
	static CReaverBot_Balfura* Create();
	static CReaverBot_Balfura* Create(const _float3 vPos);
	virtual CGameObject* Clone(void* Arg = nullptr);

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
};

END