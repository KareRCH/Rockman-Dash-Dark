#pragma once

#include "PrimitiveComponent.h"

BEGIN(Engine)

class ENGINE_DLL CVIBufferComp abstract : public CPrimitiveComponent
{
	DERIVED_CLASS(CPrimitiveComponent, CVIBufferComp)
protected:
	explicit CVIBufferComp(ID3D11Device* pGraphicDev);
	explicit CVIBufferComp(const CVIBufferComp& rhs);
	virtual ~CVIBufferComp() = default;

public:
	virtual HRESULT Initialize() PURE;
	virtual _int	Tick(const _float& fTimeDelta);
	virtual void	LateTick() PURE;
	virtual void	Render(ID3D11DeviceContext* pDeviceContext) PURE;

public:
	virtual CPrimitiveComponent*	Clone() PURE;

protected:
	virtual void	Free();

public:
	GETSET_1(_uint, m_iVtxCount, VertexCount, GET_C_REF)
	GETSET_1(_uint, m_iIndexCount, IndexCount, GET_C_REF)

protected:
	ID3D11Buffer* m_pVtxBuffer = nullptr;
	ID3D11Buffer* m_pIndexBuffer = nullptr;
	_uint m_iVtxCount = 0;
	_uint m_iIndexCount = 0;
};

END