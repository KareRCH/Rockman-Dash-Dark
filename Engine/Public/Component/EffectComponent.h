#pragma once

#include "InternalComponent.h"

#include "Component/D3D11DeviceComp.h"

BEGIN(Engine)

/// <summary>
/// ������ ���̴��� ����ϱ� ���� ���� ���� FX11�� ����ϴ� ���̴� ������Ʈ.
/// ���ɿ��� ShaderComponent�̸�.
/// �� EffectComponent �迭�� ����� ���̵��� ���� ����ϰ� �����ϰ� ����ؾ� �� �� ���δ�.
/// </summary>
class CEffectComponent : public CInternalComponent, public ID3D11DeviceComp
{
	DERIVED_CLASS(CInternalComponent, CEffectComponent)

public:
	struct FEffectCompDesc
	{
		wstring strEffectName;
	};

protected:
	explicit CEffectComponent() = default;
	explicit CEffectComponent(const CEffectComponent& rhs);
	virtual ~CEffectComponent() = default;

public:
	virtual HRESULT Initialize_Prototype(void* Arg = nullptr) override;
	virtual HRESULT Initialize(void* Arg = nullptr) override;

protected:
	static CEffectComponent* Create(const FEffectCompDesc& tDesc);
	virtual CComponent* Clone(void* Arg = nullptr) override;

protected:
	virtual void	Free();

#pragma region ����̽� ������Ʈ
public:
	virtual ID3D11Device* const D3D11Device() const override { return m_pDeviceComp->Get_Device(); }
	virtual ID3D11DeviceContext* const D3D11Context() const override { return m_pDeviceComp->Get_Context(); }

private:
	CD3D11DeviceComp* m_pDeviceComp = { nullptr };
#pragma endregion


private:
	D3DX11_TECHNIQUE_DESC				m_TechniqueDesc;
	ComPtr<ID3DX11Effect>				m_pEffect = { nullptr };
	vector<ComPtr<ID3D11InputLayout>>	m_InputLayouts;
};

END