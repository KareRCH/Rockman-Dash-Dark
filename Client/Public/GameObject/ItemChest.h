#pragma once

#include "Client_Define.h"
#include "BaseClass/GameObject.h"

BEGIN(Engine)

class CCommonModelComp;

END

BEGIN(Client)

class CItemChest : public Engine::CGameObject
{
	DERIVED_CLASS(CGameObject, CItemChest)

protected:
	explicit CItemChest();
	explicit CItemChest(const CItemChest& rhs);
	virtual ~CItemChest() = default;

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
	static CItemChest* Create();
	static CItemChest* Create(const _float3 vPos);
	CGameObject* Clone(void* Arg = nullptr);

protected:
	virtual void	Free() override;

private:
	HRESULT	Initialize_Component();

public:
	virtual FSerialData SerializeData() override;

private:
	CCommonModelComp* m_pModelComp = { nullptr };

private:
	_int			m_iTest = 0;
};

END