#pragma once

#include "System/Define/RenderMgr_Define.h"
#include "BaseClass/GameObject.h"

BEGIN(Engine)

/// <summary>
/// �������� ���� ���̾��� Rendering ������ �ο����� Ŭ������
/// ������ ������ ���� ó�� �Լ��� �޸��Ͽ� �����Ѵ�.
/// �� �����Ӹ��� �����׷쿡 �߰����־�� �մϴ�.
/// 
/// ���������� ī�޶� ��İ� ����Ʈ�� ����
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
	vector<D3D11_VIEWPORT>		m_vecViewport;			// �Ϲ� ����Ʈ ����
	vector<D3D11_VIEWPORT>		m_vecViewport_RT;		// ���� Ÿ�� ����Ʈ

};

END
