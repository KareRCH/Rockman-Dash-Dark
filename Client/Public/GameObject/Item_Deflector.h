#pragma once

#include "Client_Define.h"
#include "BaseClass/CollisionObject.h"


BEGIN(Client)

/// <summary>
/// µ∑ »πµÊ æ∆¿Ã≈€
/// </summary>
class CItem_Deflector : public CCollisionObject
{
	DERIVED_CLASS(CCollisionObject, CItem_Deflector)

protected:
	explicit CItem_Deflector();
	explicit CItem_Deflector(const CItem_Deflector& rhs);
	virtual ~CItem_Deflector() = default;

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
	static CItem_Deflector* Create();
	static CItem_Deflector* Create(const _float3 vPos);
	CGameObject* Clone(void* Arg = nullptr);

protected:
	virtual void	Free() override;

private:
	HRESULT	Initialize_Component();

public:		// √Êµπ ¿Ã∫•∆Æ
	virtual void OnCollision(CGameObject* pDst, const FContact* pContact);
	virtual void OnCollisionEntered(CGameObject* pDst, const FContact* pContact);
	virtual void OnCollisionExited(CGameObject* pDst);

private:
	CCommonModelComp* m_pModelComp = { nullptr };
};

END