#pragma once

#include "GameObject.h"
#include "System/Define/PipelineMgr_Define.h"

BEGIN(Engine)

/// <summary>
/// 카메라 베이스 클래스
/// </summary>
class ENGINE_DLL CCamera abstract : public CGameObject
{
	DERIVED_CLASS(CGameObject, CCamera)

public:
	struct FCameraDesc
	{
		_float3		vEye, vAt, vUp;
		_float		fFov, fAspect, fNear, fFar;
	};

protected:
	explicit CCamera();
	explicit CCamera(const CCamera& rhs);
	virtual ~CCamera() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize();
	virtual void	Priority_Tick(const _float& fTimeDelta);
	virtual _int	Tick(const _float& fTimeDelta);
	virtual void	Late_Tick(const _float& fTimeDelta);
	virtual void	Render();

protected:
	virtual void	Free();

public:
	void Apply_ViewProjMatrix();

protected:
	ECamType	m_eCamType;			// 사용 카메라 종류 (원근, 직교)
	ECamNum		m_eCamID;			// 사용 카메라 넘버 (0~7, 뷰포트와 동기화됨)

	_float3		m_vAt;				// 바라보는 지점
	_float		m_fFov;				// 시야각
	_float		m_fAspect;			// 비율
	_float		m_fNear;			// 카메라 최소거리
	_float		m_fFar;				// 카메라 최대거리

	_float4x4	m_matView;			// 저장용 뷰 행렬
	_float4x4	m_matProj;			// 저장용 투영 행렬
};

END