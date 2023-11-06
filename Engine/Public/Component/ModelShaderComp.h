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
	explicit CModelShaderComp(const DX11DEVICE_T tDevice);
	explicit CModelShaderComp(const CModelShaderComp& rhs);
	virtual ~CModelShaderComp() = default;

public:
	virtual HRESULT Initialize();
	virtual HRESULT	Initialize(HWND hWnd);
	virtual void	PriorityTick();
	virtual _int	Tick(const _float& fTimeDelta);
	virtual void	LateTick() {}
	virtual void	Render() {}
	virtual void	Render(const MATRIX_BUFFER_T& tMatrixBuf, const LIGHT_BUFFER_T& tLightBuf);

public:
	static CModelShaderComp* Create(const DX11DEVICE_T tDevice, HWND hWnd);
	virtual CPrimitiveComponent* Clone(void* Arg);

protected:
	virtual void	Free();

public:
	HRESULT Initialize_Shader(HWND hWnd, const _tchar* vsFileName, const _tchar* psFileName);
	HRESULT	Set_ShaderParameter(MATRIX_BUFFER_T tMatrixBuf, LIGHT_BUFFER_T tLightBuf);
	void	Render_Shader(_int iIndexCount);

public:
	GETSET_1(ID3D11ShaderResourceView*, m_pTexture, Texture, SET__C)

private:
	ID3D11SamplerState* m_pSamplereState = nullptr;
	ID3D11ShaderResourceView* m_pTexture = nullptr;
};



END