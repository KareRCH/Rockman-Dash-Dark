#pragma once


#include "GameObject.h"

BEGIN(Engine)

class CColliderComponent;
class FContact;

/// <summary>
/// �ϳ��� �浹ü�� ������ ������Ʈ Ŭ����
/// �Ϲ����� ��ü�� �ƴ� �浹�� �ʿ��� ��ü�� �� Ŭ������ ��� �޴´�.
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


protected: // �浹
	virtual void	OnCollision(CGameObject* pDst, const FContact* const pContact) PURE;
	virtual void	OnCollisionEntered(CGameObject* pDst, const FContact* const pContact) PURE;
	virtual void	OnCollisionExited(CGameObject* pDst) PURE;

protected:
	CColliderComponent* m_pColliderComp = { nullptr };
};

END