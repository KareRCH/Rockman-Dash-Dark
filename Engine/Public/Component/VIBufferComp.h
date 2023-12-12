#pragma once

#include "Component/InternalComponent.h"
#include "Component/Interface/IVIBufferComp.h"
#include "Component/D3D11DeviceComp.h"

BEGIN(Engine)

class ENGINE_DLL CVIBufferComp abstract : public CInternalComponent
{
	DERIVED_CLASS(CInternalComponent, CVIBufferComp)

protected:
	explicit CVIBufferComp() = default;
	explicit CVIBufferComp(const CVIBufferComp& rhs);
	virtual ~CVIBufferComp() = default;

public:
	virtual HRESULT	Initialize_Prototype(void* Arg = nullptr) override;
	virtual HRESULT Initialize(void* Arg = nullptr) override;

public:
	virtual CComponent*	Clone(void* Arg = nullptr) PURE;

protected:
	virtual void	Free();

#pragma region 디바이스 컴포넌트
protected:
	ID3D11Device* const D3D11Device() const { return m_pDeviceComp->Get_Device(); }
	ID3D11DeviceContext* const D3D11Context() const { return m_pDeviceComp->Get_Context(); }

private:
	CD3D11DeviceComp* m_pDeviceComp = { nullptr };
#pragma endregion


public:
	HRESULT Create_Buffer(_Inout_ ID3D11Buffer** ppBuffer);

public:
	GETSET_1(_uint, m_iNumVertices, VertexCount, GET_C_REF)
	GETSET_1(_uint, m_iNumIndices, IndexCount, GET_C_REF)

protected:
	ComPtr<ID3D11Buffer> m_pVB = { nullptr };
	ComPtr<ID3D11Buffer> m_pIB = { nullptr };

	D3D11_BUFFER_DESC		m_BufferDesc;
	D3D11_SUBRESOURCE_DATA	m_SubResourceData;

protected:
	_uint	m_iNumVertices = { 0 };
	_uint	m_iStride = { 0 };
	_uint	m_iNumVertexBuffers = { 0 };

	_uint	m_iNumIndices = { 0 };
	_uint	m_iIndexStride = { 0 };

protected:
	DXGI_FORMAT					m_eIndexFormat = { };
	D3D11_PRIMITIVE_TOPOLOGY	m_eTopology = {};

};

END