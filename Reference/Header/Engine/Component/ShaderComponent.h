#pragma once

#include "Component/InternalComponent.h"

#include "Component/D3D11DeviceComp.h"

BEGIN(Engine)

/// <summary>
/// ���̴� �ڵ带 �������ϱ� ���� ���̴� Ŭ����
/// �ʼ��� ���Ǵ� ���̴� �İ�ü�� ���� ������ ������.
/// �� �ʿ��� ������ �ִٸ� ��ӹ޾� �߰������� �����Ͽ� ����Ѵ�.
/// </summary>
class ENGINE_DLL CShaderComponent abstract : public CInternalComponent, public ID3D11DeviceComp
{
	DERIVED_CLASS(CInternalComponent, CShaderComponent)
protected:
	explicit CShaderComponent() = default;
	explicit CShaderComponent(const CShaderComponent& rhs);
	virtual ~CShaderComponent() = default;

	struct FInitData
	{
		HWND hWnd;
	};

public:
	virtual HRESULT Initialize_Prototype(void* Arg = nullptr) override;
	virtual HRESULT Initialize(void* Arg = nullptr) PURE;

protected:
	virtual CComponent* Clone(void* Arg = nullptr) PURE;

protected:
	virtual void	Free();

protected:
	void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hWnd, const _tchar* pShaderFileName);

public:
	GETSET_1(_uint, m_iIndexCount, IndexCount, SET_C)

protected:
	ComPtr<ID3D11VertexShader>	m_pVertexShader = { nullptr };
	ComPtr<ID3D11PixelShader>	m_pPixelShader = { nullptr };
	ComPtr<ID3D11InputLayout>	m_pLayout = { nullptr };
	ComPtr<ID3D11Buffer>		m_pMatrixBuffer = { nullptr };
	ComPtr<ID3D11Buffer>		m_pCameraBuffer = { nullptr };
	ComPtr<ID3D11Buffer>		m_pLightBuffer = { nullptr };
	_uint						m_iIndexCount = 0;


#pragma region ����̽� ������Ʈ
public:
	virtual ID3D11Device* const D3D11Device() const override { return m_pDeviceComp->Get_Device(); }
	virtual ID3D11DeviceContext* const D3D11Context() const override { return m_pDeviceComp->Get_Context(); }

private:
	CD3D11DeviceComp* m_pDeviceComp = { nullptr };
#pragma endregion


};

END