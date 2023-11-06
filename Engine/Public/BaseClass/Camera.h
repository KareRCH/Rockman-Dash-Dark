#pragma once

#include "GameObject.h"

BEGIN(Engine)

/// <summary>
/// 카메라 베이스 클래스
/// </summary>
class ENGINE_DLL CCamera abstract : public CGameObject
{
	DERIVED_CLASS(CGameObject, CCamera)
protected:
	explicit CCamera(const DX11DEVICE_T tDevice);
	explicit CCamera(const CCamera& rhs);
	virtual ~CCamera() = default;

public:
	virtual HRESULT Initialize();
	virtual void	PriorityTick();
	virtual _int	Tick(const _float& fTimeDelta);
	virtual void	LateTick();
	virtual void	Render();

protected:
	virtual void	Free();

protected:
	_float3 m_vAt;
	_float3 m_vUp;
	_matrix m_matPersView;
	_matrix	m_matPersProj;
};

END