#pragma once

#include "Client_Define.h"
#include "BaseClass/GameObject.h"

#include "Component/BoxModelComp.h"
#include "Component/CylinderModelComp.h"
#include "GameObject/GameObjectFactory.h"
#include "GameObject/ItemChest.h"

BEGIN(Engine)


END


BEGIN(Client)

/// <summary>
/// 스카이 박스를 렌더링 하는 객체이다.
/// </summary>
class CSkyBox final : public CGameObject
{
	DERIVED_CLASS(CGameObject, CSkyBox)

public:
	static const _uint g_ClassID = ECast(EObjectIDExt::SkyBox);

protected:
	explicit CSkyBox();
	explicit CSkyBox(const CSkyBox& rhs);
	virtual ~CSkyBox() = default;

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
	static	CSkyBox*		Create();
	static	CSkyBox*		Create(FSerialData& InputData);
	virtual CGameObject*	Clone(void* Arg = nullptr);
	virtual CGameObject*	Clone(FSerialData& InputData);

protected:
	virtual void	Free() override;

public:
	virtual FSerialData SerializeData_Prototype() override;
	virtual FSerialData SerializeData() override;

private:
	HRESULT	Initialize_Component();
	// 시리얼 데이터로 초기화시 사용
	HRESULT	Initialize_Component(FSerialData& InputData);

private:
	CCylinderModelComp* m_pModelComp = { nullptr };

};

template <>
struct TObjectExtTrait<CSkyBox::g_ClassID>
{
	using Class = CSkyBox;
};

END