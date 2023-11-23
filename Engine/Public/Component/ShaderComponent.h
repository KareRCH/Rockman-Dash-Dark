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
	explicit CShaderComponent() = default;
	explicit CShaderComponent(const CShaderComponent& rhs);
	virtual ~CShaderComponent() = default;

	struct FInitData
	{
		HWND hWnd;
	};

public:
	virtual HRESULT Initialize(void* Arg = nullptr) PURE;
	virtual void	Priority_Tick(const _float& fTimeDelta) PURE;
	virtual _int	Tick(const _float& fTimeDelta) PURE;
	virtual void	Late_Tick(const _float& fTimeDelta) PURE;
	virtual void	Render() PURE;

protected:
	virtual CComponent* Clone(void* Arg = nullptr) PURE;

protected:
	virtual void	Free();

protected:
	void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hWnd, const _tchar* pShaderFileName);

public:
	GETSET_1(_uint, m_iIndexCount, IndexCount, SET_C)

protected:
	ComPtr<ID3D11VertexShader> m_pVertexShader = nullptr;
	ComPtr<ID3D11PixelShader>	m_pPixelShader = nullptr;
	ID3D11InputLayout*			m_pLayout = nullptr;
	ID3D11Buffer*				m_pMatrixBuffer = nullptr;
	ID3D11Buffer*				m_pCameraBuffer = nullptr;
	ID3D11Buffer*				m_pLightBuffer = nullptr;
	_uint m_iIndexCount = 0;

};

END