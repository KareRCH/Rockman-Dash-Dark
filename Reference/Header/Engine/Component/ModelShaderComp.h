#pragma once

#include "ShaderComponent.h"

BEGIN(Engine)

/// <summary>
/// 모델용 셰이더, 테스트 버전
/// </summary>
class ENGINE_DLL CModelShaderComp final : public CShaderComponent
{
	DERIVED_CLASS(CShaderComponent, CModelShaderComp)
protected:
	explicit CModelShaderComp(ID3D11Device* pDevice);
	explicit CModelShaderComp(const CModelShaderComp& rhs);
	virtual ~CModelShaderComp() = default;

public:
	virtual HRESULT Initialize();
	virtual HRESULT	Initialize(HWND hWnd);
	virtual _int	Tick(const _float& fTimeDelta);
	virtual void	LateTick() {}
	virtual void	Render(ID3D11DeviceContext* pDeviceContext) {}
	virtual void	Render(ID3D11DeviceContext* pDeviceContext, const _matrix& matWorld, const _matrix& matView, const _matrix& matProj);

public:
	static CModelShaderComp* Create(ID3D11Device* pDevice, HWND hWnd);
	virtual CPrimitiveComponent* Clone();

protected:
	virtual void	Free();

public:
	HRESULT Initialize_Shader(HWND hWnd, const _tchar* vsFileName, const _tchar* psFileName);
	HRESULT	Set_ShaderParameter(ID3D11DeviceContext* pDeviceContext, XMMATRIX matWorld, XMMATRIX matView, XMMATRIX matProj);
	void	Render_Shader(ID3D11DeviceContext* pDeviceContext, _int iIndexCount);


private:
	ID3D11SamplerState* m_pSamplereState = nullptr;
};



END