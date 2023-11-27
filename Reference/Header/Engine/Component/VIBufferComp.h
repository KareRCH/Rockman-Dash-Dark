#pragma once

#include "Component/InternalComponent.h"
#include "Component/Interface/IVIBufferComp.h"
#include "Component/D3D11DeviceComp.h"

BEGIN(Engine)

class ENGINE_DLL CVIBufferComp abstract : public CInternalComponent, public ID3D11DeviceComp
{
	DERIVED_CLASS(CInternalComponent, CVIBufferComp)

protected:
	explicit CVIBufferComp() = default;
	explicit CVIBufferComp(const CVIBufferComp& rhs);
	virtual ~CVIBufferComp() = default;

public:
	virtual HRESULT	Initialize_Prototype(void* Arg = nullptr) override;
	virtual HRESULT Initialize(void* Arg = nullptr);

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




#pragma region 디바이스 컴포넌트
protected:
	virtual ID3D11Device* const D3D11Device() const override { return m_pDeviceComp->Get_Device(); }
	virtual ID3D11DeviceContext* const D3D11Context() const override { return m_pDeviceComp->Get_Context(); }

private:
	CD3D11DeviceComp* m_pDeviceComp = { nullptr };
#pragma endregion

};

END