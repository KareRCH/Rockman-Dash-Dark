#pragma once

#include "Component/PrimitiveComponent.h"

BEGIN(Engine)

/// <summary>
/// ���̴� �ڵ带 �������ϱ� ���� ���̴� Ŭ����
/// �ʼ��� ���Ǵ� ���̴� �İ�ü�� ���� ������ ������.
/// �� �ʿ��� ������ �ִٸ� ��ӹ޾� �߰������� �����Ͽ� ����Ѵ�.
/// </summary>
class ENGINE_DLL CShaderComponent abstract : public CPrimitiveComponent
{
	DERIVED_CLASS(CPrimitiveComponent, CShaderComponent)
protected:
	explicit CShaderComponent(ID3D11Device* pDevice);
	explicit CShaderComponent(const CShaderComponent& rhs);
	virtual ~CShaderComponent() = default;

public:
	virtual HRESULT Initialize() PURE;
	virtual _int	Tick(const _float& fTimeDelta) PURE;
	virtual void	LateTick() PURE;
	virtual void	Render(ID3D11DeviceContext* pDeviceContext) PURE;

protected:
	virtual CPrimitiveComponent* Clone() PURE;

protected:
	virtual void	Free();

protected:
	void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hWnd, const _tchar* pShaderFileName);

protected:
	ID3D11VertexShader* m_pVertexShader = nullptr;
	ID3D11PixelShader*	m_pPixelShader = nullptr;
	ID3D11InputLayout*	m_pLayout = nullptr;
	ID3D11Buffer*		m_pMatrixBuffer = nullptr;
};

END