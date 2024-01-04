#pragma once


#include "GameObject.h"

BEGIN(Engine)

class CColliderComponent;
class FContact;

/// <summary>
/// 하나의 충돌체를 가지는 오브젝트 클래스
/// 일반적인 객체가 아닌 충돌이 필요한 객체는 이 클래스를 상속 받는다.
/// </summary>
class ENGINE_DLL CCollisionObject abstract : public CGameObject
{
	DERIVED_CLASS(CGameObject, CCollisionObject)

protected:
	explicit CCollisionObject();
	explicit CCollisionObject(const CCollisionObject& rhs);
	virtual ~CCollisionObject() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* Arg = nullptr) override;
	virtual void	Priority_Tick(const _float& fTimeDelta) override;
	virtual void	Tick(const _float& fTimeDelta) override;
	virtual void	Late_Tick(const _float& fTimeDelta) override;
	virtual HRESULT	Render() override;

public:
	virtual CGameObject*	Clone(void* Arg = nullptr) PURE;

protected:
	virtual void			Free() override;

protected:
	HRESULT Initialize_Component();


protected: // 충돌
	virtual void	OnCollision(CGameObject* pDst, const FContact* const pContact) PURE;
	virtual void	OnCollisionEntered(CGameObject* pDst, const FContact* const pContact) PURE;
	virtual void	OnCollisionExited(CGameObject* pDst) PURE;

protected:
	CColliderComponent* m_pColliderComp = { nullptr };
};

END