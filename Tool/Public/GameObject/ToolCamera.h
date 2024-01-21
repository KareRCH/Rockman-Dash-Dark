#pragma once

#include "Tool_Define.h"
#include "BaseClass/Camera.h"

BEGIN(Tool)

class CToolCamera : public Engine::CCamera
{
	DERIVED_CLASS(CCamera, CToolCamera)
protected:
	explicit CToolCamera();
	explicit CToolCamera(const CToolCamera& rhs);
	virtual ~CToolCamera() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* Arg = nullptr) override;
	virtual void	Priority_Tick(const _float& fTimeDelta) override;
	virtual void	Tick(const _float& fTimeDelta) override;
	virtual void	Late_Tick(const _float& fTimeDelta) override;
	virtual HRESULT	Render() override;

public:
	static CToolCamera* Create();
	CGameObject* Clone(void* Arg = nullptr) override;

protected:
	virtual void	Free() override;

public:
	GETSET_2(_bool, m_bIsCanMove, IsCanMove, GET_C_REF, SET_C_REF)

private:
	_bool		m_bIsCanMove = false;
	_float		m_fSpeed = { 5.f };
};

END