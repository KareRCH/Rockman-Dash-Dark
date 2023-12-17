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

#pragma region ����̽� ������Ʈ
protected:
	ID3D11Device* const D3D11Device() const { return m_pDeviceComp->Get_Device(); }
	ID3D11DeviceContext* const D3D11Context() const { return m_pDeviceComp->Get_Context(); }

private:
	CD3D11DeviceComp* m_pDeviceComp = { nullptr };
#pragma endregion


public:
	HRESULT Create_Buffer(_Inout_ ID3D11Buffer** ppBuffer);
	void Update_VBuffer(void* pData, _uint iCount);
	// ������� ������ Ȯ���س��� ��. �迭��ü�� �ε��� �������, ��Һ� ����� �ʿ��ϴ�.
	HRESULT Copy_VBufferToArray(void* pArray, size_t iSize, size_t iSizePerIndex);
	HRESULT Copy_IBufferToArray(void* pArray, size_t iSize);

public:
	GETSET_1(_uint, m_iNumVertices, VertexCount, GET_C_REF)
	GETSET_1(_uint, m_iNumIndices, IndexCount, GET_C_REF)

protected:
	ComPtr<ID3D11Buffer> m_pVB = { nullptr };		// ���� ����
	ComPtr<ID3D11Buffer> m_pIB = { nullptr };		// �ε��� ����

	D3D11_BUFFER_DESC		m_BufferDesc;			// ���� ����
	D3D11_SUBRESOURCE_DATA	m_SubResourceData;		// �ʱ�ȭ ������

protected:
	_uint	m_iNumVertices = { 0 };			// ������ ����
	_uint	m_iVertexStride = { 0 };		// ������ �� ��Ʈ ����Ʈ ��
	_uint	m_iNumVertexBuffers = { 0 };	// ���� ���۴� �������� �� �� �ִ�. �ε����� ������ �ϳ�

	_uint	m_iNumIndices = { 0 };			// �ε����� ����
	_uint	m_iIndexStride = { 0 };			// �ε����� �� ��Ʈ ����Ʈ ��

protected:
	DXGI_FORMAT					m_eIndexFormat = { };	// �ε��� ���� ���
	D3D11_PRIMITIVE_TOPOLOGY	m_eTopology = {};		// �ﰢ�� �׸��� ���

};

END