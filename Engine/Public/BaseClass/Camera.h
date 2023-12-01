#pragma once

#include "GameObject.h"

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

protected:
	_float3		m_vAt;
	_float3		m_vUp;
	_float		m_fFov;
	_float		m_fAspect;

	_float4x4	m_matPersView;
	_float4x4	m_matPersProj;
};

END