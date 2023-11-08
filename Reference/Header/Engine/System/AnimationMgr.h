#pragma once

#include "Base.h"

BEGIN(Engine)

/// <summary>
/// �ִϸ��̼� �����͸� �����ϴ� Ŭ����
/// </summary>
class CAnimationMgr final : public CBase
{
	DERIVED_CLASS(CBase, CAnimationMgr)
private:
	explicit CAnimationMgr(const DX11DEVICE_T tDevice);
	explicit CAnimationMgr(const CAnimationMgr& rhs) = delete;
	virtual ~CAnimationMgr() = default;

public:
	HRESULT					Initialize();

public:
	static CAnimationMgr*	Create(const DX11DEVICE_T tDevice);

private:
	virtual void			Free() override;

private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pDeviceContext = { nullptr };
};

END