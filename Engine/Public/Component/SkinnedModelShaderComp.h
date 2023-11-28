#pragma once

#include "ShaderComponent.h"

BEGIN(Engine)

class CSkinnedModelShaderComp : public CShaderComponent
{
	DERIVED_CLASS(CShaderComponent, CSkinnedModelShaderComp)

protected:
	explicit CSkinnedModelShaderComp() = default;
	explicit CSkinnedModelShaderComp(const CSkinnedModelShaderComp& rhs);
	virtual ~CSkinnedModelShaderComp() = default;

public:
	virtual HRESULT Initialize_Prototype(void* Arg = nullptr) override;
	virtual HRESULT Initialize(void* Arg = nullptr);
	virtual void	Render(const MATRIX_BUFFER_T& tMatrixBuf, const CAMERA_BUFFER_T tCameraBuf, const BONE_COMMON_BUFFER_T tBoneBuf, const LIGHT_BUFFER_T& tLightBuf);

public:
	static CSkinnedModelShaderComp* Create(HWND hWnd);
	virtual CComponent* Clone(void* Arg = nullptr);

protected:
	virtual void	Free();

public:
	HRESULT Initialize_Shader(HWND hWnd, const wstring& strVertexShaderKey, const wstring& strPixelShaderKey);
	HRESULT	Set_ShaderParameter(MATRIX_BUFFER_T tMatrixBuf, CAMERA_BUFFER_T tCameraBuf,
		BONE_COMMON_BUFFER_T tBoneBuf, LIGHT_BUFFER_T tLightBuf);
	void	Render_Shader(_int iIndexCount);

public:
	GETSET_1(ID3D11ShaderResourceView*, m_pTextureView, Texture, SET__C)

private:
	ComPtr<ID3D11SamplerState> m_pSamplereState = { nullptr };		// 샘플러
	ComPtr<ID3D11ShaderResourceView> m_pTextureView = { nullptr };	// 셰이더용 텍스처

	ComPtr<ID3D11Buffer>	m_pBoneBuffer = { nullptr };			// 뼈 전용 버퍼
};

END