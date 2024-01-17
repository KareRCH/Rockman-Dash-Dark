#pragma once

#include "Base.h"

BEGIN(Engine)

class CRenderTargetMgr : public CBase
{
	DERIVED_CLASS(CBase, CRenderTargetMgr)

private:
	explicit CRenderTargetMgr(const DX11DEVICE_T tDevice);
	explicit CRenderTargetMgr(const CRenderTargetMgr& rhs) = delete;
	virtual ~CRenderTargetMgr() = default;

public:
	HRESULT		Initialize();
	HRESULT		Begin_MRT(const wstring& strMRTTag);
	HRESULT		End_MRT();
	HRESULT		Bind_ShaderResource(const wstring& strTargetTag, class CEffectComponent* pEffect, const _char* pConstantName);

#ifdef _DEBUG
public:
	HRESULT		Ready_Debug(const wstring& strTargetTag, _float fX, _float fY, _float fSizeX, _float fSizeY);
	HRESULT		Render_Debug(const wstring& strMRTTag, class CEffectComponent* pEffect, class CRectBufferComp* pVIBuffer);
#endif

public:
	static CRenderTargetMgr* Create(const DX11DEVICE_T tDevice);

private:
	virtual void	Free();



private:
	ComPtr<ID3D11Device> m_pDevice = { nullptr };
	ComPtr<ID3D11DeviceContext> m_pContext = { nullptr };

private:
	map<const wstring, class CRenderTarget*>			m_RenderTargets;
	map<const wstring, list<class CRenderTarget*>>		m_MRTs;

public:
	HRESULT Add_RenderTarget(const wstring& strTargetTag, _uint iSizeX, _uint iSizeY, DXGI_FORMAT ePixelFormat, const _float4 vClearColor);
	HRESULT Add_MRT(const wstring& strMRTTag, const wstring& strTargetTag);
	HRESULT Resize_RenderTargets(_uint iResizeWidth, _uint iResizeHeight);

private:
	class CRenderTarget* Find_RenderTarget(const wstring& strTargetTag);
	list<class CRenderTarget*>* Find_MRT(const wstring& strMRTTag);


};

END