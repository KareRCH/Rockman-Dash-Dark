#include "System/RenderTargetMgr.h"
#include "System/Data/RenderTarget.h"

#include "System/GameInstance.h"
#include "Component/VIBufferComp.h"

CRenderTargetMgr::CRenderTargetMgr(const DX11DEVICE_T tDevice)
	: m_pDevice(tDevice.pDevice), m_pContext(tDevice.pDeviceContext)
{
}

HRESULT CRenderTargetMgr::Initialize()
{
	return S_OK;
}

HRESULT CRenderTargetMgr::Begin_MRT(const wstring& strMRTTag, ID3D11DepthStencilView* pDSV)
{
	ID3D11ShaderResourceView* pSRV[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = {
		nullptr
	};

	m_pContext->PSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, pSRV);

	/* strMRTTag에 모여있는 렌더타겟들을 장치에 순차적으로 바인딩한다. */
	list<CRenderTarget*>* pMRTList = Find_MRT(strMRTTag);
	if (nullptr == pMRTList)
		return E_FAIL;
	ID3D11RenderTargetView* pRTVs[8];

	_uint		iNumRTVs = { 0 };

	for (auto& pRenderTarget : *pMRTList)
	{
		pRenderTarget->Clear();
		pRTVs[iNumRTVs++] = pRenderTarget->Get_RTV();
	}

	if (nullptr != pDSV)
		m_pContext->ClearDepthStencilView(pDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

	m_pContext->OMSetRenderTargets(iNumRTVs, pRTVs, nullptr == pDSV ? GI()->Get_DSV() : pDSV);

	return S_OK;
}

HRESULT CRenderTargetMgr::End_MRT()
{
	ID3D11RenderTargetView* pRTVs[] = {
		GI()->Get_BackBufferRTV()
	};

	/* 최초의 상태(백버퍼)로 되돌린다. */
	m_pContext->OMSetRenderTargets(1, pRTVs, GI()->Get_DSV());

	return S_OK;
}

HRESULT CRenderTargetMgr::Bind_ShaderResource(const wstring& strTargetTag, CEffectComponent* pEffect, const _char* pConstantName)
{
	CRenderTarget* pRenderTarget = Find_RenderTarget(strTargetTag);
	if (nullptr == pRenderTarget)
		return E_FAIL;

	return pRenderTarget->Bind_ShaderResource(pEffect, pConstantName);
}

#ifdef _DEBUG
HRESULT CRenderTargetMgr::Ready_Debug(const wstring& strTargetTag, _float fX, _float fY, _float fSizeX, _float fSizeY)
{
	CRenderTarget* pRenderTarget = Find_RenderTarget(strTargetTag);
	if (nullptr == pRenderTarget)
		return E_FAIL;

	return pRenderTarget->Ready_Debug(fX, fY, fSizeX, fSizeY);
}

HRESULT CRenderTargetMgr::Render_Debug(const wstring& strMRTTag, CEffectComponent* pEffect, CRectBufferComp* pVIBuffer)
{
	list<CRenderTarget*>* pMRTList = Find_MRT(strMRTTag);
	if (nullptr == pMRTList)
		return E_FAIL;

	for (auto& pRenderTarget : *pMRTList)
	{
		pRenderTarget->Render_Debug(pEffect, Cast<CVIBufferComp*>(pVIBuffer));
	}
	return S_OK;
}
#endif // _DEBUG


CRenderTargetMgr* CRenderTargetMgr::Create(const DX11DEVICE_T tDevice)
{
	ThisClass* pInstance = new ThisClass(tDevice);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("RenderMgr Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRenderTargetMgr::Free()
{
	for (auto& Pair : m_MRTs)
	{
		for (auto& pRenderTarget : Pair.second)
			Safe_Release(pRenderTarget);
		Pair.second.clear();
	}
	m_MRTs.clear();

	for (auto& Pair : m_RenderTargets)
		Safe_Release(Pair.second);

	m_RenderTargets.clear();
}

HRESULT CRenderTargetMgr::Add_RenderTarget(const wstring& strTargetTag, _uint iSizeX, _uint iSizeY, DXGI_FORMAT ePixelFormat, const _float4 vClearColor)
{
	if (nullptr != Find_RenderTarget(strTargetTag))
		return E_FAIL;

	CRenderTarget* pRenderTarget = CRenderTarget::Create({ m_pDevice, m_pContext }, iSizeX, iSizeY, ePixelFormat, vClearColor);
	if (nullptr == pRenderTarget)
		return E_FAIL;

	m_RenderTargets.emplace(strTargetTag, pRenderTarget);

	return S_OK;
}

HRESULT CRenderTargetMgr::Add_MRT(const wstring& strMRTTag, const wstring& strTargetTag)
{
	CRenderTarget* pRenderTarget = Find_RenderTarget(strTargetTag);
	if (nullptr == pRenderTarget)
		return E_FAIL;

	list<CRenderTarget*>* pMRTList = Find_MRT(strMRTTag);
	if (nullptr == pMRTList)
	{
		list<CRenderTarget*>	MRTList;
		MRTList.push_back(pRenderTarget);

		m_MRTs.emplace(strMRTTag, MRTList);
	}
	else
		pMRTList->push_back(pRenderTarget);

	Safe_AddRef(pRenderTarget);

	return S_OK;
}

HRESULT CRenderTargetMgr::Resize_RenderTargets(_uint iResizeWidth, _uint iResizeHeight)
{
	for (auto iter = m_RenderTargets.begin(); iter != m_RenderTargets.end(); ++iter)
	{
		(*iter).second->Resize_RenderTarget(iResizeWidth, iResizeHeight);
	}

	return S_OK;
}

ID3D11Texture2D* CRenderTargetMgr::Find_RenderTargetTexture2D(const wstring& strTargetTag)
{
	CRenderTarget* pRenderTarget = Find_RenderTarget(strTargetTag);
	if (nullptr == pRenderTarget)
		return nullptr;

	return pRenderTarget->Get_Texture2D();
}

CRenderTarget* CRenderTargetMgr::Find_RenderTarget(const wstring& strTargetTag)
{
	auto	iter = m_RenderTargets.find(strTargetTag);

	if (iter == m_RenderTargets.end())
		return nullptr;

	return iter->second;
}

list<class CRenderTarget*>* CRenderTargetMgr::Find_MRT(const wstring& strMRTTag)
{
	auto	iter = m_MRTs.find(strMRTTag);

	if (iter == m_MRTs.end())
		return nullptr;

	return &iter->second;
}
