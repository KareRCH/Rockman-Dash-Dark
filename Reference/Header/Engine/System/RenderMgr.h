#pragma once

#include "BaseClass/GameObject.h"

BEGIN(Engine)

enum class EGCAMERA_INDEX : _uint
{
	ONE, TWO, THREE, FOUR, SIZE
};

// ����Ÿ�ٿ� ����Ʈ ����ü
enum class EVIEWPORT : _uint
{
	ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, SIZE
};

// ����Ÿ�ٿ� ����Ʈ ����ü
enum class EVIEWPORT_RT : _uint
{
	NORMAL, DEPTH, HEIGHT, SIZE
};

enum class ERENDER_TYPE : _uint
{
	PERSPECTIVE,
	ORTHOGONAL,
	SIZE
};
using ERENDER_T = ERENDER_TYPE;

/// <summary>
/// �������� ���� ���̾��� Rendering ������ �ο����� Ŭ������
/// ������ ������ ���� ó�� �Լ��� �޸��Ͽ� �����Ѵ�.
/// �� �����Ӹ��� �����׷쿡 �߰����־�� �մϴ�.
/// </summary>
class CRenderMgr final : public CBase
{
	DERIVED_CLASS(CBase, CRenderMgr)

private:
	explicit CRenderMgr();
	virtual ~CRenderMgr() = default;

public:
	HRESULT			Initialize(const _uint iWidth = 1280U, const _uint iHeight = 720U);
	void			Render(ID3D11DeviceContext* pGraphicDev);

public:
	static CRenderMgr* Create(const _uint iWidth = 1280U, const _uint iHeight = 720U);

private:
	virtual void	Free();

public:
	void			Add_RenderGroup(ERENDER_T eType, CGameObject* pGameObject);
	void			Clear_RenderGroup();

	void			Render_Perspective(ID3D11DeviceContext* pGraphicDev);
	void			Render_Orthogonal(ID3D11DeviceContext* pGraphicDev);



public:
	GETSET_1(HRESULT, m_hReadyResult, ReadyResult, GET_C_REF)

private:
	HRESULT			m_hReadyResult;

private:
	list<CGameObject*>		m_RenderGroup[Cast_EnumDef(ERENDER_T::SIZE)];

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
	_matrix		m_matPersView[Cast_EnumDef(EGCAMERA_INDEX::SIZE)];			// ���ٿ� �� ���
	_matrix		m_matPersProj[Cast_EnumDef(EGCAMERA_INDEX::SIZE)];			// ���ٿ� ���� ���
	_matrix		m_matOrthoView[Cast_EnumDef(EGCAMERA_INDEX::SIZE)];			// ������ �� ���
	_matrix		m_matOrthoProj[Cast_EnumDef(EGCAMERA_INDEX::SIZE)];			// UI�� �������� ����

public:
	GETSET_1(vector<D3D11_VIEWPORT>,	m_vecViewport,		VecViewport,				GET_REF)
	D3D11_VIEWPORT&	Get_Viewport(_uint value) { return m_vecViewport[value]; }

	GETSET_1(vector<D3D11_VIEWPORT>,	m_vecViewport_RT,	VecViewport_RenderTarget,	GET_REF)

private:
	vector<D3D11_VIEWPORT>		m_vecViewport;			// �Ϲ� ����Ʈ ����
	vector<D3D11_VIEWPORT>		m_vecViewport_RT;		// ���� Ÿ�� ����Ʈ

};

END
