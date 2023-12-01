#pragma once

#include "Component/PrimitiveComponent.h"
#include "Component/TransformComponent.h"
#include "Component/PipelineComp.h"
#include "BaseClass/GameObject.h"

BEGIN(Engine)


/// <summary>
/// 내부적으로 트랜스폼 컴포넌트를 포함하는 클래스
/// 이 클래스를 상속받으면 트랜스폼 속성을 같이 가지게 된다.
/// </summary>
class ENGINE_DLL CSceneComponent abstract : public CPrimitiveComponent, public ITransform
{
	DERIVED_CLASS(CPrimitiveComponent, CSceneComponent)
protected:
	explicit CSceneComponent() = default;
	explicit CSceneComponent(const CSceneComponent& rhs);
	virtual ~CSceneComponent() = default;

public:
	virtual HRESULT	Initialize_Prototype(void* Arg = nullptr) override;
	virtual HRESULT Initialize(void* Arg = nullptr) override;
	virtual void	Priority_Tick(const _float& fTimeDelta) PURE;
	virtual _int	Tick(const _float& fTimeDelta);
	virtual void	Late_Tick(const _float& fTimeDelta) PURE;
	virtual void	Render() PURE;

public:
	virtual CComponent* Clone(void* Arg = nullptr) PURE;

protected:
	virtual void	Free() override;


#pragma region 씬 컴포넌트 계층
	// 씬 컴포넌트는 계층관계를 가진다. 자식을 여럿 가질 수 있으며, 그에 대한 기능을 제공한다.
public:
	GETSET_1(CSceneComponent*, m_pParentSceneComp, ParentSceneComp, GET)

	CSceneComponent*	Get_FirstChildSceneComp();
	CSceneComponent*	Get_ChildSceneComp(_uint iIndex);
	void				Add_ChildSceneComp(CSceneComponent* const pComp);
	_bool				Insert_ChildSceneComp(_uint iIndex, CSceneComponent* const pComp);

protected:
	CSceneComponent* m_pParentSceneComp = { nullptr };		// 부모 씬 컴포넌트
	vector<CSceneComponent*>	m_vecChildSceneComp;					// 자식 씬 컴포넌트  
#pragma endregion


#pragma region 트랜스폼 컴포넌트
	// 트랜스폼 컴포넌트에 대한 함수 정의, 씬 컴포넌트는 항상 트랜스폼을 요소로 가진다.
public:
	// 반드시 초기화가 되었을 때 사용해야 함.
	virtual CTransformComponent& Transform() override { return (*m_pTransformComp); }
	virtual void Release_Transform() override { Safe_Release(m_pTransformComp); }

	_matrix Calculate_TransformMatrixFromParent();
	_float4x4 Calculate_TransformFloat4x4FromParent();

private:
	CTransformComponent* m_pTransformComp = { nullptr };			// 내부 트랜스폼 컴포넌트 포함  
#pragma endregion


#pragma region 캠뷰 컴포넌트
protected:
	CPipelineComp& PipelineComp() { return (*m_pPipelineComp); }

private:
	CPipelineComp* m_pPipelineComp = { nullptr };
#pragma endregion




};

END