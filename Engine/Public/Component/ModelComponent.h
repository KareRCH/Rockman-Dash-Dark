#pragma once

#include "SceneComponent.h"

#include "Component/VIBufferComp.h"
#include "Component/MaterialComponent.h"


BEGIN(Engine)

/// <summary>
/// Ʈ�������� ������, �⺻������ �׷����� ǥ���ϱ� ���� ���۸� ������ �� ������Ʈ.
/// �Ϲ� �� ������Ʈ�� �ܼ��� ���� ǥ���ϱ� ���� ���ۿ�, ���� ������ �����ϴ�.
/// ��, �ִϸ��̼��� �����ϴ� ��ü�� ������� �� ��ü�� ��ӹ޾� ���� �Ͻʽÿ�.
/// </summary>
class ENGINE_DLL CModelComponent abstract : public CSceneComponent, public IVIBufferComp, public IMaterialComponent
{
	DERIVED_CLASS(CSceneComponent, CModelComponent)

protected:
	explicit CModelComponent() = default;
	explicit CModelComponent(const CModelComponent& rhs);
	virtual ~CModelComponent() = default;

public:
	virtual HRESULT	Initialize_Prototype(void* Arg = nullptr) override;
	virtual HRESULT Initialize(void* Arg = nullptr) override;
	virtual void	Priority_Tick(const _float& fTimeDelta) PURE;
	virtual _int	Tick(const _float& fTimeDelta) PURE;
	virtual void	Late_Tick(const _float& fTimeDelta) PURE;
	virtual void	Render() PURE;

public:
	virtual CComponent* Clone(void* Arg = nullptr) PURE;

protected:
	virtual void	Free() override;




#pragma region VIBuffer ������Ʈ
public:
	virtual CVIBufferComp* VIBufferComp() override { return m_pVIBufferComp; }

private:
	CVIBufferComp* m_pVIBufferComp = { nullptr };
#pragma endregion




#pragma region ���͸��� ������Ʈ
public:
	virtual CMaterialComponent* MaterialComp() override { return m_pMaterialComp; }

private:
	CMaterialComponent* m_pMaterialComp = { nullptr };
#pragma endregion

	
};

END