#pragma once

#include "InternalComponent.h"

#include "Component/D3D11DeviceComp.h"

BEGIN(Engine)

/// <summary>
/// ������ ���̴��� ����ϱ� ���� ���� ���� FX11�� ����ϴ� ���̴� ������Ʈ.
/// ���ɿ��� ShaderComponent�̸�.
/// �� EffectComponent �迭�� ����� ���̵��� ���� ����ϰ� �����ϰ� ����ؾ� �� �� ���δ�.
/// �������� ���� ����� ���� �� ������Ʈ �ϳ��� �ذẻ��.
/// 
/// ������ ���������� ���ε��� �ʿ��ϴ�.
/// </summary>
class CEffectComponent : public CInternalComponent, public ID3D11DeviceComp
{
	DERIVED_CLASS(CInternalComponent, CEffectComponent)

protected:
	explicit CEffectComponent() = default;
	explicit CEffectComponent(const CEffectComponent& rhs);
	virtual ~CEffectComponent() = default;

public:
	virtual HRESULT Initialize_Prototype(void* Arg = nullptr) override;
	virtual HRESULT Initialize(void* Arg = nullptr) override;

public:
	// �ε�� �̹� ������� ���̴��� Ȱ���Ѵ�.
	// ���̴� �ڵ�� �̹� ������ �Ǿ� ������� �ִٴ� �����Ͽ� �ε��Ѵ�.
	static CEffectComponent* Create();
	virtual CComponent* Clone(void* Arg = nullptr) override;

protected:
	virtual void	Free();

#pragma region ����̽� ������Ʈ
protected:
	virtual ID3D11Device* const D3D11Device() const override { return m_pDeviceComp->Get_Device(); }
	virtual ID3D11DeviceContext* const D3D11Context() const override { return m_pDeviceComp->Get_Context(); }

private:
	CD3D11DeviceComp* m_pDeviceComp = { nullptr };
#pragma endregion

	
public:	// �ܺο� �Լ�
	HRESULT Bind_Effect(const wstring& strEffectKey);
	HRESULT Unbind_Effect();

public:
	// �н� ����
	HRESULT Begin(_uint iPassIndex);
	// ��� ���ε�
	HRESULT Bind_Matrix(const _char* pConstantName, const _float4x4* pMatrix);
	HRESULT Bind_Matrices(const _char* pConstantName, const _float4x4* pMatrix, _uint iNumMatrices);
	// �ؽ�ó ���ε�
	HRESULT Bind_SRV(const _char* pConstantName, ID3D11ShaderResourceView* pSRV);
	HRESULT Bind_SRVs(const _char* pConstantName, ID3D11ShaderResourceView** ppSRV, _uint iNumTextures);
	// �Ϲ� ���� ���ε�
	HRESULT Bind_RawValue(const _char* pConstantName, const void* pData, _uint iSize);


private:
	FEffectData* m_pEffectData = { nullptr };	// ����Ʈ ������
};

END