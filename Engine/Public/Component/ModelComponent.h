#pragma once

#include "SceneComponent.h"

#include "Component/MaterialComponent.h"


BEGIN(Engine)

/// <summary>
/// 트랜스폼을 가지고, 기본적으로 그래픽을 표시하기 위한 버퍼를 가지는 씬 컴포넌트.
/// 일반 모델 컴포넌트는 단순히 모델을 표시하기 위한 버퍼와, 재질 정도를 가집니다.
/// 뼈, 애니메이션을 포함하는 객체를 만드려면 이 객체를 상속받아 정의 하십시오.
/// </summary>
class ENGINE_DLL CModelComponent : public CSceneComponent, public IMaterialComponent
{
	DERIVED_CLASS(CSceneComponent, CModelComponent)

protected:
	explicit CModelComponent() = default;
	explicit CModelComponent(const CModelComponent& rhs);
	virtual ~CModelComponent() = default;

public:
	virtual HRESULT	Initialize_Prototype(void* Arg = nullptr) override;
	virtual HRESULT Initialize(void* Arg = nullptr) override;
	virtual void	Priority_Tick(const _float& fTimeDelta);
	virtual void	Tick(const _float& fTimeDelta);
	virtual void	Late_Tick(const _float& fTimeDelta);
	virtual HRESULT	Render() PURE;

public:
	virtual CComponent* Clone(void* Arg = nullptr) PURE;

protected:
	virtual void	Free() override;


#pragma region 머터리얼 컴포넌트
public:
	virtual CMaterialComponent* MaterialComp() override { return m_pMaterialComp; }

private:
	CMaterialComponent* m_pMaterialComp = { nullptr };
#pragma endregion

	
};

END