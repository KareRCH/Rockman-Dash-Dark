#pragma once

#include "Client_Define.h"
#include "BaseClass/GameObject.h"

#include "Component/BoxModelComp.h"


BEGIN(Engine)

END


BEGIN(Client)

/// <summary>
/// 스카이 박스를 렌더링 하는 객체이다.
/// </summary>
class CSkyBox final : public CGameObject
{
	DERIVED_CLASS(CGameObject, CSkyBox)
protected:
	explicit CSkyBox();
	explicit CSkyBox(const CSkyBox& rhs);
	virtual ~CSkyBox() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* Arg = nullptr) override;
	virtual void	Priority_Tick(const _float& fTimeDelta) override;
	virtual void	Tick(const _float& fTimeDelta) override;
	virtual void	Late_Tick(const _float& fTimeDelta) override;
	virtual HRESULT	Render() override;

public:
	static CSkyBox* Create();
	CGameObject* Clone(void* Arg = nullptr) override;

protected:
	virtual void	Free() override;

private:
	HRESULT Initialize_Component();

private:
	CBoxModelComp* m_pModelComp = { nullptr };

};

END