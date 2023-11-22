#pragma once

#include "SceneComponent.h"

BEGIN(Engine)

class ENGINE_DLL CVIBufferComp abstract : public CSceneComponent
{
	DERIVED_CLASS(CSceneComponent, CVIBufferComp)
protected:
	explicit CVIBufferComp() = default;
	explicit CVIBufferComp(const CVIBufferComp& rhs);
	virtual ~CVIBufferComp() = default;

public:
	virtual HRESULT Initialize(void* Arg = nullptr);
	virtual void	Priority_Tick(const _float& fTimeDelta) PURE;
	virtual _int	Tick(const _float& fTimeDelta);
	virtual void	Late_Tick(const _float& fTimeDelta) PURE;
	virtual void	Render() PURE;

public:
	virtual CComponent*	Clone(void* Arg = nullptr) PURE;

protected:
	virtual void	Free();

public:
	GETSET_1(_uint, m_iVtxCount, VertexCount, GET_C_REF)
	GETSET_1(_uint, m_iIndexCount, IndexCount, GET_C_REF)

protected:
	ComPtr<ID3D11Buffer> m_pVtxBuffer = { nullptr };
	ComPtr<ID3D11Buffer> m_pIndexBuffer = { nullptr };
	_uint m_iVtxCount = 0;
	_uint m_iIndexCount = 0;
};

END