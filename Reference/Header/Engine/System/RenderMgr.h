#pragma once

#include "System/Define/RenderMgr_Define.h"
#include "BaseClass/GameObject.h"

#include "Utility/DelegateTemplate.h"

BEGIN(Engine)

typedef FastDelegate0<HRESULT> DelegateDebug;
typedef MulticastDelegate<DelegateDebug> MultiDeleDebug;

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
	HRESULT			Render();

public:
	static CRenderMgr* Create(const DX11DEVICE_T tDevice, const _uint iWidth = 1280U, const _uint iHeight = 720U);

private:
	virtual void	Free();

public:
	void			Add_RenderGroup(ERenderGroup eType, CGameObject* pGameObject);
	void			Clear_RenderGroup();

	HRESULT			Render_Priority();
	HRESULT			Render_NonLight();
	HRESULT			Render_NonBlend();
	HRESULT			Render_Blend();
	HRESULT			Render_UI();
	HRESULT			Render_PostProcess();

	HRESULT			Render_LightAcc();
	HRESULT			Render_Deferred();
#ifdef _DEBUG
private:
	HRESULT			Render_Debug();
#endif

public:
	GETSET_1(HRESULT, m_hReadyResult, ReadyResult, GET_C_REF)

private:
	HRESULT			m_hReadyResult;

private:
	list<CGameObject*>		m_RenderGroup[ECast(ERenderGroup::Size)];

private:
	ComPtr<ID3D11Device>			m_pDevice = { nullptr };
	ComPtr<ID3D11DeviceContext>		m_pDeviceContext = { nullptr };
	class CGameInstance*			m_pGI = { nullptr };
	list<class CGameObject*>		m_RenderObjects[RENDER_END];

public:
	void Toggle_Deferred() { m_bIsDeferred = !m_bIsDeferred; }

private:
	_bool					m_bIsDeferred = { false };

	class CEffectComponent* m_pEffect = { nullptr };
	class CRectBufferComp* m_pVIBuffer = { nullptr };

	_float4x4								m_WorldMatrix;
	_float4x4								m_ViewMatrix, m_ProjMatrix;

public:
	GETSET_1(vector<D3D11_VIEWPORT>, m_vecViewport,	VecViewport, GET_REF)
	D3D11_VIEWPORT&	Get_Viewport(_uint value) { return m_vecViewport[value]; }

	GETSET_1(vector<D3D11_VIEWPORT>, m_vecViewport_RT, VecViewport_RenderTarget, GET_REF)

private:
	_uint						m_iNumViewPorts = { 0 };
	vector<D3D11_VIEWPORT>		m_vecViewport;			// �Ϲ� ����Ʈ ����
	vector<D3D11_VIEWPORT>		m_vecViewport_RT;		// ���� Ÿ�� ����Ʈ

public:
	void Add_DebugEvent(DelegateDebug DebugEvent) { if (m_bIsDebugDraw) m_DebugEvent.Add(DebugEvent); }
	void Toggle_DebugDraw() { m_bIsDebugDraw = !m_bIsDebugDraw; }

private:
	MultiDeleDebug		m_DebugEvent;
	_bool				m_bIsDebugDraw = { false };


#pragma region ���������� ������Ʈ
protected:
	CPipelineComp& PipelineComp() { return (*m_pPipelineComp); }

private:
	CPipelineComp* m_pPipelineComp = { nullptr };
#pragma endregion

};

END
