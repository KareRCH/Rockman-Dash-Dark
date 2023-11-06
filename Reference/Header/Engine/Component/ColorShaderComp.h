#pragma once

#include "ShaderComponent.h"

BEGIN(Engine)

class ENGINE_DLL CColorShaderComp final : public CShaderComponent
{
	DERIVED_CLASS(CShaderComponent, CColorShaderComp)
protected:
	explicit CColorShaderComp(const DX11DEVICE_T tDevice);
	explicit CColorShaderComp(const CColorShaderComp& rhs);
	virtual ~CColorShaderComp() = default;

public:
	virtual HRESULT Initialize();
	virtual HRESULT	Initialize(HWND hWnd);
	virtual void	PriorityTick();
	virtual _int	Tick(const _float& fTimeDelta);
	virtual void	LateTick() {}
	virtual void	Render() {}
	virtual void	Render(const _matrix& matWorld, const _matrix& matView, const _matrix& matProj);

public:
	static CColorShaderComp*		Create(const DX11DEVICE_T tDevice, HWND hWnd);
	virtual CPrimitiveComponent*	Clone(void* Arg);

protected:
	virtual void	Free();

public:
	HRESULT Initialize_Shader(HWND hWnd, const _tchar* vsFileName, const _tchar* psFileName);
	HRESULT	Set_ShaderParameter(XMMATRIX matWorld, XMMATRIX matView, XMMATRIX matProj);
	void	Render_Shader(_int iIndexCount);
	
};

END