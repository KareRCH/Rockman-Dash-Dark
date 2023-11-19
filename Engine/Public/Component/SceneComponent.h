#pragma once

#include "Component/PrimitiveComponent.h"
#include "Component/TransformComponent.h"

BEGIN(Engine)


/// <summary>
/// 내부적으로 트랜스폼 컴포넌트를 포함하는 클래스
/// 이 클래스를 상속받으면 트랜스폼 속성을 같이 가지게 된다.
/// </summary>
class ENGINE_DLL CSceneComponent abstract : public CPrimitiveComponent
{
	DERIVED_CLASS(CPrimitiveComponent, CSceneComponent)
protected:
	explicit CSceneComponent(const DX11DEVICE_T tDevice);
	explicit CSceneComponent(const CSceneComponent& rhs);
	virtual ~CSceneComponent();

public:
	virtual HRESULT Initialize();
	virtual void	Priority_Tick(const _float& fTimeDelta) PURE;
	virtual _int	Tick(const _float& fTimeDelta);
	virtual void	Late_Tick(const _float& fTimeDelta) PURE;
	virtual void	Render() PURE;

public:
	virtual CComponent* Clone(void* Arg = nullptr) PURE;

protected:
	virtual void	Free();

public:
	GETSET_1(CSceneComponent*, m_pParentComp, ParentComp, GET)
	CSceneComponent*	Get_FirstChildComp();
	CSceneComponent*	Get_ChildComp(_uint iIndex);
	void				Add_Child(CSceneComponent* const pComp);
	_bool				Insert_Child(_uint iIndex, CSceneComponent* const pComp);

protected:
	CSceneComponent*			m_pParentComp = { nullptr };		// 부모 컴포넌트
	list<CSceneComponent*>		m_listChildSceneComp;				// 자식 컴포넌트


public:		// 트랜스폼 컴포넌트에 대한 함수 정의
	const _float3	Get_Position() const { return m_pTransformComp->Get_Position(); }
	void			Set_Position(const _float3& value) { m_pTransformComp->Set_Position(value); }

	const _float3	Get_Rotation() const { return m_pTransformComp->Get_Rotation(); }
	void			Set_Rotation(const _float3& value) { m_pTransformComp->Set_Rotation(value); }

	const _float3	Get_Scale() const { return m_pTransformComp->Get_Scale(); }
	void			Set_Scale(const _float3& value) { m_pTransformComp->Set_Scale(value); }

	void			Calculate_TransformFromParent(_matrix* matTransform)
	{
		m_pTransformComp->Calculate_TransformFromParent(matTransform);
	}

	const void		Set_Transform(_float4x4 value) { m_pTransformComp->Set_Transform(value); }
	const _float4x4* Get_Transform() const { return m_pTransformComp->Get_Transform(); }

	const _float3	Get_RightFromTransform() const { return m_pTransformComp->Get_Right(); }
	const _float3	Get_UpFromTransform() const { return m_pTransformComp->Get_Up(); }
	const _float3	Get_LookFromTransform() const { return m_pTransformComp->Get_Look(); }

private:
	CTransformComponent* m_pTransformComp = { nullptr };			// 내부 트랜스폼 컴포넌트 포함


};

END