#pragma once

#include "Base.h"

BEGIN(Engine)

class CParticleMgr final : public CBase
{
	DERIVED_CLASS(CBase, CParticleMgr)

private:
	explicit CParticleMgr(const DX11DEVICE_T tDevice);
	explicit CParticleMgr(const CParticleMgr& rhs) = delete;
	virtual ~CParticleMgr() = default;

public:
	HRESULT			Initialize();

public:
	static CParticleMgr* Create(const DX11DEVICE_T tDevice);

private:
	virtual void	Free();

private:
	ComPtr<ID3D11Device>			m_pDevice = { nullptr };
	ComPtr<ID3D11DeviceContext>		m_pDeviceContext = { nullptr };
};

END