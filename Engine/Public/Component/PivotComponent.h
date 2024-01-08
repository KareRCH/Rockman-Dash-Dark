#pragma once

#include "SceneComponent.h"

BEGIN(Engine)

/// <summary>
/// 오브젝트에 붙여 특정 위치를 표현하기 위한 컴포넌트
/// </summary>
class ENGINE_DLL CPivotComponent : public CSceneComponent
{
	DERIVED_CLASS(CSceneComponent, CPivotComponent)

protected:
	explicit CPivotComponent();
	explicit CPivotComponent(const CPivotComponent& rhs);
	virtual ~CPivotComponent() = default;

public:
	virtual HRESULT Initialize_Prototype(void* Arg = nullptr);
	virtual HRESULT Initialize(void* Arg = nullptr);
	virtual void	Priority_Tick(const _float& fTimeDelta);
	virtual void	Tick(const _float& fTimeDelta);
	virtual void	Late_Tick(const _float& fTimeDelta);
	virtual HRESULT	Render();

public:
	static	CPivotComponent*	Create();
	virtual CComponent*			Clone(void* Arg = nullptr) override;

protected:
	virtual void				Free();


#pragma region 트랜스폼 컴포넌트
	// 트랜스폼 컴포넌트에 대한 함수 정의
public:
	// 반드시 초기화가 되었을 때 사용해야 함.
	inline CTransformComponent& OffsetTransform() { return (*m_pOffsetTransformComp); }

private:	// 게임 오브젝트 기본 정의 컴포넌트
	CTransformComponent* m_pOffsetTransformComp = { nullptr };
#pragma endregion

};

END