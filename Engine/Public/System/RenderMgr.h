#pragma once

#include "BaseClass/GameObject.h"

BEGIN(Engine)

enum class ECameraIndex : _uint
{
	One, Two, Three, Four, Size
};

// 렌더타겟용 뷰포트 열거체
enum class EViewportIndex : _uint
{
	One, Two, Three, Four, Five, Six, Seven, Eight, Size
};

// 렌더타겟용 뷰포트 열거체
enum class EViewportRT : _uint
{
	Normal, Depth, Height, Size
};

enum class ERenderGroup : _uint
{
	Priority,		// 버퍼 없이 가장 먼저 렌더링 되는 그룹
	Alpha,			// 알파 그룹
	NonAlpha,		// 알파 필요없는 디퍼드 그룹
	UI,				// UI 그룹
	PostProcess,	// 후처리 그룹
	Size
};

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

public:		// 카메라 행렬저장용
	void Set_PerspectiveViewMatrix(const _uint iCam, const _matrix& matPersView);
	const _matrix& Get_PerspectiveViewMatrix(const _uint iCam) const { return m_matPersView[iCam]; }

	void Set_PerspectiveProjMatrix(const _uint iCam, const _matrix& matPersProj);
	const _matrix& Get_PerspectiveProjMatrix(const _uint iCam) const { return m_matPersProj[iCam]; }

	void Set_OrthogonalViewMatrix(const _uint iCam, const _matrix& matOrthoView);
	const _matrix& Get_OrthogonalViewMatrix(const _uint iCam) const { return m_matOrthoView[iCam]; }

	void Set_OrthogonalProjMatrix(const _uint iCam, const _matrix& matPersProj);
	const _matrix& Get_OrthogonalProjMatrix(const _uint iCam) const { return m_matOrthoProj[iCam]; }

private:	// 렌더러의 위치 속성은 뷰를 기반으로
	_matrix		m_matPersView[ECast(ECameraIndex::Size)];			// 원근용 뷰 행렬
	_matrix		m_matPersProj[ECast(ECameraIndex::Size)];			// 원근용 투영 행렬
	_matrix		m_matOrthoView[ECast(ECameraIndex::Size)];			// 직교용 뷰 행렬
	_matrix		m_matOrthoProj[ECast(ECameraIndex::Size)];			// UI용 직교투영 범위

public:
	GETSET_1(vector<D3D11_VIEWPORT>, m_vecViewport,	VecViewport, GET_REF)
	D3D11_VIEWPORT&	Get_Viewport(_uint value) { return m_vecViewport[value]; }

	GETSET_1(vector<D3D11_VIEWPORT>, m_vecViewport_RT, VecViewport_RenderTarget, GET_REF)

private:
	vector<D3D11_VIEWPORT>		m_vecViewport;			// 일반 뷰포트 세팅
	vector<D3D11_VIEWPORT>		m_vecViewport_RT;		// 렌더 타겟 뷰포트

};

END
