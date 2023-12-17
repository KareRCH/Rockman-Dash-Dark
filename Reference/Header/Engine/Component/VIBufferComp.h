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
	void Update_VBuffer(void* pData, _uint iCount);
	// 사용전에 사이즈 확보해놓을 것. 배열자체의 인덱스 사이즈와, 요소별 사이즈가 필요하다.
	HRESULT Copy_VBufferToArray(void* pArray, size_t iSize, size_t iSizePerIndex);
	HRESULT Copy_IBufferToArray(void* pArray, size_t iSize);

public:
	GETSET_1(_uint, m_iNumVertices, VertexCount, GET_C_REF)
	GETSET_1(_uint, m_iNumIndices, IndexCount, GET_C_REF)

protected:
	ComPtr<ID3D11Buffer> m_pVB = { nullptr };		// 정점 버퍼
	ComPtr<ID3D11Buffer> m_pIB = { nullptr };		// 인덱스 버퍼

	D3D11_BUFFER_DESC		m_BufferDesc;			// 버퍼 설명
	D3D11_SUBRESOURCE_DATA	m_SubResourceData;		// 초기화 데이터

protected:
	_uint	m_iNumVertices = { 0 };			// 정점의 개수
	_uint	m_iVertexStride = { 0 };		// 정점의 한 세트 바이트 수
	_uint	m_iNumVertexBuffers = { 0 };	// 정점 버퍼는 여러개가 될 수 있다. 인덱스는 무조건 하나

	_uint	m_iNumIndices = { 0 };			// 인덱스의 개수
	_uint	m_iIndexStride = { 0 };			// 인덱스의 한 세트 바이트 수

protected:
	DXGI_FORMAT					m_eIndexFormat = { };	// 인덱스 버퍼 방식
	D3D11_PRIMITIVE_TOPOLOGY	m_eTopology = {};		// 삼각형 그리기 방식

};

END