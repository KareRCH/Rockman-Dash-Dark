#pragma once

#include "BaseClass/GameObject.h"

BEGIN(Engine)

enum class ECameraIndex : _uint
{
	One, Two, Three, Four, Size
};

// ����Ÿ�ٿ� ����Ʈ ����ü
enum class EViewportIndex : _uint
{
	One, Two, Three, Four, Five, Six, Seven, Eight, Size
};

// ����Ÿ�ٿ� ����Ʈ ����ü
enum class EViewportRT : _uint
{
	Normal, Depth, Height, Size
};

enum class ERenderGroup : _uint
{
	Priority,		// ���� ���� ���� ���� ������ �Ǵ� �׷�
	Alpha,			// ���� �׷�
	NonAlpha,		// ���� �ʿ���� ���۵� �׷�
	UI,				// UI �׷�
	PostProcess,	// ��ó�� �׷�
	Size
};

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

public:		// ī�޶� ��������
	void Set_PerspectiveViewMatrix(const _uint iCam, const _matrix& matPersView);
	const _matrix& Get_PerspectiveViewMatrix(const _uint iCam) const { return m_matPersView[iCam]; }

	void Set_PerspectiveProjMatrix(const _uint iCam, const _matrix& matPersProj);
	const _matrix& Get_PerspectiveProjMatrix(const _uint iCam) const { return m_matPersProj[iCam]; }

	void Set_OrthogonalViewMatrix(const _uint iCam, const _matrix& matOrthoView);
	const _matrix& Get_OrthogonalViewMatrix(const _uint iCam) const { return m_matOrthoView[iCam]; }

	void Set_OrthogonalProjMatrix(const _uint iCam, const _matrix& matPersProj);
	const _matrix& Get_OrthogonalProjMatrix(const _uint iCam) const { return m_matOrthoProj[iCam]; }

private:	// �������� ��ġ �Ӽ��� �並 �������
	_matrix		m_matPersView[ECast(ECameraIndex::Size)];			// ���ٿ� �� ���
	_matrix		m_matPersProj[ECast(ECameraIndex::Size)];			// ���ٿ� ���� ���
	_matrix		m_matOrthoView[ECast(ECameraIndex::Size)];			// ������ �� ���
	_matrix		m_matOrthoProj[ECast(ECameraIndex::Size)];			// UI�� �������� ����

public:
	GETSET_1(vector<D3D11_VIEWPORT>, m_vecViewport,	VecViewport, GET_REF)
	D3D11_VIEWPORT&	Get_Viewport(_uint value) { return m_vecViewport[value]; }

	GETSET_1(vector<D3D11_VIEWPORT>, m_vecViewport_RT, VecViewport_RenderTarget, GET_REF)

private:
	vector<D3D11_VIEWPORT>		m_vecViewport;			// �Ϲ� ����Ʈ ����
	vector<D3D11_VIEWPORT>		m_vecViewport_RT;		// ���� Ÿ�� ����Ʈ

};

END
