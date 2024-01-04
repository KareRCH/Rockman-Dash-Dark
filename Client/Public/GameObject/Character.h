#pragma once

#include "Client_Define.h"
#include "BaseClass/CollisionObject.h"

BEGIN(Client)

/// <summary>
/// ĳ������ ���̽� Ŭ����.
/// � ������ ĳ���͵� �� �� �ִ�.
/// </summary>
class CCharacter abstract : public CCollisionObject
{
	DERIVED_CLASS(CCollisionObject, CCharacter)

protected:
	explicit CCharacter();
	explicit CCharacter(const CCharacter& rhs);
	virtual ~CCharacter() = default;

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

private:
	HRESULT			Initialize_Component();

public:		// �浹 �̺�Ʈ
	virtual void OnCollision(CGameObject* pDst, const FContact* pContact) PURE;
	virtual void OnCollisionEntered(CGameObject* pDst, const FContact* pContact) PURE;
	virtual void OnCollisionExited(CGameObject* pDst) PURE;
};

END