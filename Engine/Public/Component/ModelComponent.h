#pragma once

#include "SceneComponent.h"

#include "Component/MaterialComponent.h"


BEGIN(Engine)

/// <summary>
/// Ʈ�������� ������, �⺻������ �׷����� ǥ���ϱ� ���� ���۸� ������ �� ������Ʈ.
/// �Ϲ� �� ������Ʈ�� �ܼ��� ���� ǥ���ϱ� ���� ���ۿ�, ���� ������ �����ϴ�.
/// ��, �ִϸ��̼��� �����ϴ� ��ü�� ������� �� ��ü�� ��ӹ޾� ���� �Ͻʽÿ�.
/// </summary>
class ENGINE_DLL CModelComponent : public CSceneComponent
{
	DERIVED_CLASS(CSceneComponent, CModelComponent)

protected:
	explicit CModelComponent();
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


public:
	// ���ϴ� �н��� �׸� �� �ֵ��� �߰��� �Լ�
	void Set_ActivePass(_uint iPass);
	// �ʿ������ �н��� ���� �� �ֵ��� �����ϴ� �Լ�
	void Unset_ActivePass(_uint iPass);
	// ��Ƽ�� �н� �ʱ�ȭ
	void Reset_ActivePass();

protected:
	_uint				m_iNumActivePasses = { 0 };
	vector<_uint>		m_vecActivePasses;
	
};

END