#pragma once

#include "BaseClass/Camera.h"
#include "Client_Define.h"

BEGIN(Client)

/// <summary>
/// 게임 내에서 유동적으로 쓰이는 카메라
/// </summary>
class CDynamicCamera final : public CCamera
{
	DERIVED_CLASS(CCamera, CDynamicCamera)
protected:
	explicit CDynamicCamera(ID3D11Device* const pDevice);
	explicit CDynamicCamera(const CDynamicCamera& rhs);
	virtual ~CDynamicCamera() = default;

public:
	virtual HRESULT Initialize();
	virtual _int	Tick(const _float& fTimeDelta);
	virtual void	LateTick();
	virtual void	Render(ID3D11DeviceContext* const pDeviceContext);

public:
	static CDynamicCamera* Create(ID3D11Device* const pDevice);

protected:
	virtual void	Free();
};

END