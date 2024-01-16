#pragma once

#include "SceneComponent.h"
#include "Utility/ClassID.h"

BEGIN(Engine)

/// <summary>
/// ������Ʈ�� �ٿ� Ư�� ��ġ�� ǥ���ϱ� ���� ������Ʈ
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
	// ������Ÿ�� ���ۿ� �Լ�
	virtual FSerialData SerializeData_Prototype();
	// Ŭ�δ� ���� �Լ�
	virtual FSerialData SerializeData();

public:
	static	CPivotComponent* Create();
	static	CPivotComponent* Create(FSerialData& InputData);
	virtual CComponent* Clone(void* Arg = nullptr) override;
	virtual CComponent* Clone(FSerialData& InputData);

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

template <>
struct TComponentTrait<CPivotComponent::g_ClassID>
{
	using Class = CPivotComponent;
};

END