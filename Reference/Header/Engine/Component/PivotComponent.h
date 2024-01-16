#pragma once

#include "SceneComponent.h"
#include "Utility/ClassID.h"

BEGIN(Engine)

/// <summary>
/// 오브젝트에 붙여 특정 위치를 표현하기 위한 컴포넌트
/// </summary>
class ENGINE_DLL CPivotComponent : public CSceneComponent
{
	DERIVED_CLASS(CSceneComponent, CPivotComponent)

public:
	static const _uint g_ClassID = ECast(EComponentID::Pivot);

protected:
	explicit CPivotComponent();
	explicit CPivotComponent(const CPivotComponent& rhs);
	virtual ~CPivotComponent() = default;

public:
	virtual HRESULT	Initialize_Prototype(void* Arg = nullptr) override;
	virtual HRESULT Initialize_Prototype(FSerialData& InputData);
	virtual HRESULT Initialize(void* Arg = nullptr) override;
	virtual HRESULT Initialize(FSerialData& InputData);
	virtual void	Priority_Tick(const _float& fTimeDelta);
	virtual void	Tick(const _float& fTimeDelta);
	virtual void	Late_Tick(const _float& fTimeDelta);
	virtual HRESULT	Render();

public:
	// 프로토타입 제작용 함수
	virtual FSerialData SerializeData_Prototype();
	// 클로닝 전용 함수
	virtual FSerialData SerializeData();

public:
	static	CPivotComponent* Create();
	static	CPivotComponent* Create(FSerialData& InputData);
	virtual CComponent* Clone(void* Arg = nullptr) override;
	virtual CComponent* Clone(FSerialData& InputData);

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

template <>
struct TComponentTrait<CPivotComponent::g_ClassID>
{
	using Class = CPivotComponent;
};

END