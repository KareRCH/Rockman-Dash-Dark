#pragma once

#include "ShaderComponent.h"

BEGIN(Engine)

class ENGINE_DLL CColorShaderComp final : public CShaderComponent
{
	DERIVED_CLASS(CShaderComponent, CColorShaderComp)
protected:
	explicit CColorShaderComp() = default;
	explicit CColorShaderComp(const CColorShaderComp& rhs);
	virtual ~CColorShaderComp() = default;

public:
	virtual HRESULT Initialize_Prototype(void* Arg = nullptr) override;
	virtual HRESULT Initialize(void* Arg = nullptr) override;
	virtual HRESULT	Initialize(HWND hWnd);
	virtual void	Priority_Tick(const _float& fTimeDelta) override;
	virtual _int	Tick(const _float& fTimeDelta) override;
	virtual void	Late_Tick(const _float& fTimeDelta) override {}
	virtual void	Render() override {}
	virtual void	Render(const _matrix& matWorld, const _matrix& matView, const _matrix& matProj);

public:
	static CColorShaderComp*		Create(HWND hWnd);
	virtual CComponent*				Clone(void* Arg = nullptr) override;

protected:
	virtual void	Free() override;

public:
	HRESULT Initialize_Shader(HWND hWnd, const _tchar* vsFileName, const _tchar* psFileName);
	HRESULT	Set_ShaderParameter(XMMATRIX matWorld, XMMATRIX matView, XMMATRIX matProj);
	void	Render_Shader(_int iIndexCount);
	
};

END