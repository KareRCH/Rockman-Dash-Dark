#pragma once

#include "VIBufferComp.h"

BEGIN(Engine)

class ENGINE_DLL CTriBufferComp : public CVIBufferComp
{
	DERIVED_CLASS(CVIBufferComp, CTriBufferComp)
protected:
	explicit CTriBufferComp(ID3D11Device* pGraphicDev);
	explicit CTriBufferComp(const CTriBufferComp& rhs);
	virtual ~CTriBufferComp() = default;

public:
	virtual HRESULT Initialize();
	virtual _int	Tick(const _float& fTimeDelta);
	virtual void	LateTick();
	virtual void	Render(ID3D11DeviceContext* pDeviceContext);

public:
	virtual CPrimitiveComponent* Clone();

protected:
	virtual void	Free();
};

END