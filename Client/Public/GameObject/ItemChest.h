#pragma once

#include "Client_Define.h"
#include "GameObject/Character_Common.h"
#include "GameObject/GameObjectFactory.h"

BEGIN(Engine)

class CCommonModelComp;

END

BEGIN(Client)

class CItemChest : public CCharacter_Common
{
	DERIVED_CLASS(CCharacter_Common, CItemChest)

public:
	static const _uint g_ClassID = ECast(EObjectIDExt::ItemChest);

protected:
	explicit CItemChest();
	explicit CItemChest(const CItemChest& rhs);
	virtual ~CItemChest() = default;

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
	static	CItemChest* Create();
	static	CItemChest* Create(FSerialData& InputData);
	virtual CGameObject* Clone(void* Arg = nullptr);
	virtual CGameObject* Clone(FSerialData& InputData);

protected:
	virtual void	Free() override;

private:
	HRESULT	Initialize_Component();
	// �ø��� �����ͷ� �ʱ�ȭ�� ���
	HRESULT	Initialize_Component(FSerialData& InputData);

public:
	virtual FSerialData SerializeData_Prototype() override;
	virtual FSerialData SerializeData() override;

private:
	CCommonModelComp* m_pModelComp = { nullptr };

private:
	_int			m_iTest = 0;
};

template <>
struct TObjectExtTrait<CItemChest::g_ClassID>
{
	using Class = CItemChest;
};

END