#pragma once

#include "BaseClass/Camera.h"
#include "Client_Define.h"

BEGIN(Client)

/// <summary>
/// 게임 내에서 유동적으로 쓰이는 카메라
/// </summary>
class CDynamicCamera final : public Engine::CCamera
{
	DERIVED_CLASS(CCamera, CDynamicCamera)
protected:
	explicit CDynamicCamera();
	explicit CDynamicCamera(const CDynamicCamera& rhs);
	virtual ~CDynamicCamera() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* Arg = nullptr) override;
	virtual void	Priority_Tick(const _float& fTimeDelta) override;
	virtual void	Tick(const _float& fTimeDelta) override;
	virtual void	Late_Tick(const _float& fTimeDelta) override;
	virtual HRESULT	Render() override;

public:
	static CDynamicCamera* Create();
	CGameObject* Clone(void* Arg = nullptr) override;

protected:
	virtual void	Free() override;
};

END