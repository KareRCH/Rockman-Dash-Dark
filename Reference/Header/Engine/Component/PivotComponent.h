#pragma once

#include "SceneComponent.h"

BEGIN(Engine)

/// <summary>
/// ������Ʈ�� �ٿ� Ư�� ��ġ�� ǥ���ϱ� ���� ������Ʈ
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


#pragma region Ʈ������ ������Ʈ
	// Ʈ������ ������Ʈ�� ���� �Լ� ����
public:
	// �ݵ�� �ʱ�ȭ�� �Ǿ��� �� ����ؾ� ��.
	inline CTransformComponent& OffsetTransform() { return (*m_pOffsetTransformComp); }

private:	// ���� ������Ʈ �⺻ ���� ������Ʈ
	CTransformComponent* m_pOffsetTransformComp = { nullptr };
#pragma endregion

};

END