#pragma once

#include "GameObject.h"

BEGIN(Engine)

/// <summary>
/// ī�޶� ���̽� Ŭ����
/// </summary>
class ENGINE_DLL CCamera abstract : public CGameObject
{
	DERIVED_CLASS(CGameObject, CCamera)
protected:
	explicit CCamera(ID3D11Device* const pDevice);
	explicit CCamera(const CCamera& rhs);
	virtual ~CCamera() = default;

public:
	virtual HRESULT Initialize();
	virtual _int	Tick(const _float& fTimeDelta);
	virtual void	LateTick();
	virtual void	Render(ID3D11DeviceContext* const pDeviceContext);

protected:
	virtual void	Free();

protected:
	_float3 m_vAt;
	_float3 m_vUp;
	_matrix m_matPersView;
	_matrix	m_matPersProj;
};

END