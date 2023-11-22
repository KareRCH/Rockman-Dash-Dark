#pragma once

#include "Component/PrimitiveComponent.h"
#include "Component/TransformComponent.h"
#include "Component/Interface/ITransform.h"

BEGIN(Engine)


/// <summary>
/// ���������� Ʈ������ ������Ʈ�� �����ϴ� Ŭ����
/// �� Ŭ������ ��ӹ����� Ʈ������ �Ӽ��� ���� ������ �ȴ�.
/// </summary>
class ENGINE_DLL CSceneComponent abstract : public CPrimitiveComponent, public ITransform
{
	DERIVED_CLASS(CPrimitiveComponent, CSceneComponent)
protected:
	explicit CSceneComponent() = default;
	explicit CSceneComponent(const CSceneComponent& rhs);
	virtual ~CSceneComponent() = default;

public:
	virtual HRESULT	Initialize_Prototype(void* Arg = nullptr);
	virtual HRESULT Initialize(void* Arg = nullptr);
	virtual void	Priority_Tick(const _float& fTimeDelta) PURE;
	virtual _int	Tick(const _float& fTimeDelta);
	virtual void	Late_Tick(const _float& fTimeDelta) PURE;
	virtual void	Render() PURE;

public:
	virtual CComponent* Clone(void* Arg = nullptr) PURE;

protected:
	virtual void	Free();


#pragma region �� ������Ʈ ����
	// �� ������Ʈ�� �������踦 ������. �ڽ��� ���� ���� �� ������, �׿� ���� ����� �����Ѵ�.
public:
	GETSET_1(CSceneComponent*, m_pParentSceneComp, ParentSceneComp, GET)
		CSceneComponent* Get_FirstChildSceneComp();
	CSceneComponent* Get_ChildSceneComp(_uint iIndex);
	void				Add_ChildSceneComp(CSceneComponent* const pComp);
	_bool				Insert_ChildSceneComp(_uint iIndex, CSceneComponent* const pComp);

protected:
	CSceneComponent* m_pParentSceneComp = { nullptr };		// �θ� �� ������Ʈ
	vector<CSceneComponent*>	m_vecChildSceneComp;					// �ڽ� �� ������Ʈ  
#pragma endregion


#pragma region Ʈ������
	// Ʈ������ ������Ʈ�� ���� �Լ� ����, �� ������Ʈ�� �׻� Ʈ�������� ��ҷ� ������.
public:
	// �ݵ�� �ʱ�ȭ�� �Ǿ��� �� ����ؾ� ��.
	inline virtual CTransformComponent& Transform() override { return (*m_pTransformComp); }
	inline virtual void Release_Transform() override { Safe_Release(m_pTransformComp); }

private:
	CTransformComponent* m_pTransformComp = { nullptr };			// ���� Ʈ������ ������Ʈ ����  
#pragma endregion



};

END