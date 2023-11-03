#pragma once

#include "ShaderComponent.h"

BEGIN(Engine)

class ENGINE_DLL CColorShaderComp final : public CShaderComponent
{
	DERIVED_CLASS(CShaderComponent, CColorShaderComp)
protected:
	explicit CColorShaderComp(ID3D11Device* pGraphicDev);
	explicit CColorShaderComp(const CColorShaderComp& rhs);
	virtual ~CColorShaderComp() = default;

public:
	virtual HRESULT Initialize();
	virtual HRESULT	Initialize(HWND hWnd);
	virtual _int	Tick(const _float& fTimeDelta);
	virtual void	LateTick() {}
	virtual void	Render(ID3D11DeviceContext* pDeviceContext) {}
	virtual void	Render(ID3D11DeviceContext* pDeviceContext, const _matrix& matWorld, const _matrix& matView, const _matrix& matProj);

public:
	static CColorShaderComp*		Create(ID3D11Device* pDevice, HWND hWnd);
	virtual CPrimitiveComponent*	Clone();

protected:
	virtual void	Free();

public:
	HRESULT Initialize_Shader(HWND hWnd, const _tchar* vsFileName, const _tchar* psFileName);
	HRESULT	Set_ShaderParameter(ID3D11DeviceContext* pDeviceContext, XMMATRIX matWorld, XMMATRIX matView, XMMATRIX matProj);
	void	Render_Shader(ID3D11DeviceContext* pDeviceContext, _int iIndexCount);
	
};

END