#pragma once

#include "ShaderComponent.h"

BEGIN(Engine)

/// <summary>
/// 모델용 셰이더, 테스트 버전
/// </summary>
class ENGINE_DLL CModelShaderComp final : public CShaderComponent
{
	DERIVED_CLASS(CShaderComponent, CModelShaderComp)

	struct FInitData
	{
		HWND hWnd;
	};
protected:
	explicit CModelShaderComp() = default;
	explicit CModelShaderComp(const CModelShaderComp& rhs);
	virtual ~CModelShaderComp() = default;

public:
	virtual HRESULT Initialize_Prototype(void* Arg = nullptr) override;
	virtual HRESULT Initialize(void* Arg = nullptr);
	virtual void	Render(const MATRIX_BUFFER_T& tMatrixBuf, const CAMERA_BUFFER_T tCameraBuf, const BONE_COMMON_BUFFER_T tBoneBuf, const LIGHT_BUFFER_T& tLightBuf);

public:
	static CModelShaderComp* Create(HWND hWnd);
	virtual CComponent* Clone(void* Arg = nullptr);

protected:
	virtual void	Free();

public:
	HRESULT Initialize_Shader(HWND hWnd, const wstring& strVertexShaderKey, const wstring& strPixelShaderKey);
	HRESULT	Set_ShaderParameter(MATRIX_BUFFER_T tMatrixBuf, CAMERA_BUFFER_T tCameraBuf,
		BONE_COMMON_BUFFER_T tBoneBuf, LIGHT_BUFFER_T tLightBuf);
	void	Render_Shader(_int iIndexCount);

public:
	GETSET_1(ID3D11ShaderResourceView*, m_pTexture, Texture, SET__C)

private:
	ComPtr<ID3D11SamplerState> m_pSamplereState = { nullptr };
	ComPtr<ID3D11ShaderResourceView> m_pTexture = { nullptr };

	ComPtr<ID3D11Buffer>	m_pBoneBuffer = { nullptr };
};

END