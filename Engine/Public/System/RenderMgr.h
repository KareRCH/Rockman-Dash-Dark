#pragma once

#include "System/Define/RenderMgr_Define.h"
#include "BaseClass/GameObject.h"

BEGIN(Engine)

/// <summary>
/// 렌더러는 기존 레이어의 Rendering 역할을 부여받은 클래스로
/// 렌더링 목적에 따라 처리 함수를 달리하여 수행한다.
/// 매 프레임마다 렌더그룹에 추가해주어야 합니다.
/// 
/// 개별적으로 카메라 행렬과 뷰포트를 가짐
/// </summary>
class CRenderMgr final : public CBase
{
	DERIVED_CLASS(CBase, CRenderMgr)

private:
	explicit CRenderMgr(const DX11DEVICE_T tDevice);
	explicit CRenderMgr(const CRenderMgr& rhs) = delete;
	virtual ~CRenderMgr() = default;

public:
	HRESULT			Initialize(const _uint iWidth = 1280U, const _uint iHeight = 720U);
	void			Render();

public:
	static CRenderMgr* Create(const DX11DEVICE_T tDevice, const _uint iWidth = 1280U, const _uint iHeight = 720U);

private:
	virtual void	Free();

public:
	void			Add_RenderGroup(ERenderGroup eType, CGameObject* pGameObject);
	void			Clear_RenderGroup();

	void			Render_Priority();
	void			Render_Alpha();
	void			Render_NonAlpha();
	void			Render_UI();
	void			Render_PostProcess();

public:
	GETSET_1(HRESULT, m_hReadyResult, ReadyResult, GET_C_REF)

private:
	HRESULT			m_hReadyResult;

private:
	list<CGameObject*>		m_RenderGroup[ECast(ERenderGroup::Size)];

private:
	ComPtr<ID3D11Device>			m_pDevice = { nullptr };
	ComPtr<ID3D11DeviceContext>		m_pDeviceContext = { nullptr };

public:
	GETSET_1(vector<D3D11_VIEWPORT>, m_vecViewport,	VecViewport, GET_REF)
	D3D11_VIEWPORT&	Get_Viewport(_uint value) { return m_vecViewport[value]; }

	GETSET_1(vector<D3D11_VIEWPORT>, m_vecViewport_RT, VecViewport_RenderTarget, GET_REF)

private:
	_uint						m_iNumViewPorts = { 0 };
	vector<D3D11_VIEWPORT>		m_vecViewport;			// 일반 뷰포트 세팅
	vector<D3D11_VIEWPORT>		m_vecViewport_RT;		// 렌더 타겟 뷰포트

};

END
