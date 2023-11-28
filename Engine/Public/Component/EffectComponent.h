#pragma once

#include "InternalComponent.h"

#include "Component/D3D11DeviceComp.h"

BEGIN(Engine)

/// <summary>
/// 간편한 셰이더를 사용하기 위해 따로 만들어낸 FX11을 사용하는 셰이더 컴포넌트.
/// 성능용은 ShaderComponent이며.
/// 이 EffectComponent 계열은 고수준 셰이딩을 자주 사용하고 간편하게 사용해야 할 때 쓰인다.
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

#pragma region 디바이스 컴포넌트
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