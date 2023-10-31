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

public:
	static CColorShaderComp*		Create();
	virtual CPrimitiveComponent*	Clone();

protected:
	virtual void	Free();

public:
	HRESULT Initialize_Shader(HWND hWnd, const _tchar* vsFileName, const _tchar* psFileName);
	HRESULT	Set_ShaderParameter(ID3D11DeviceContext* pDeviceContext, XMMATRIX matWorld, XMMATRIX matView, XMMATRIX matProj);
	void	Render_Shader(ID3D11DeviceContext* pDeviceContext, _int iIndexCount);
};

END