#pragma once

#include "Client_Define.h"
#include "BaseClass/GameObject.h"


BEGIN(Engine)

class CCommonModelComp;

END


BEGIN(Client)

/// <summary>
/// 가장 기본이 되는 몬스터 리버봇, 호로꼬
/// </summary>
class CReaverBot_Horokko : public CGameObject
{
	DERIVED_CLASS(CGameObject, CReaverBot_Horokko)

protected:
	explicit CReaverBot_Horokko();
	explicit CReaverBot_Horokko(const CReaverBot_Horokko& rhs);
	virtual ~CReaverBot_Horokko() = default;

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
	static CReaverBot_Horokko* Create();
	static CReaverBot_Horokko* Create(const _float3 vPos);
	CGameObject* Clone(void* Arg = nullptr);

protected:
	virtual void	Free() override;

private:
	HRESULT	Initialize_Component();

private:
	CCommonModelComp* m_pModelComp = { nullptr };

private:
	_int			m_iTest = 0;
};

END