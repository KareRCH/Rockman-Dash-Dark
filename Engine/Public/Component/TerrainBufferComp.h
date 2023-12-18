#pragma once

#include "VIBufferComp.h"

BEGIN(Engine)


/// <summary>
/// ���̸� �ִ� �ʱ�ȭ��
/// </summary>
struct FTerrainBufInit_HeightMap
{
	wstring strHeightMapFilePath;	// ����Ʈ ���� ���
	_uint	iMaxWidth;				// �ִ� �ʺ����� ����, �ݵ�� ���簢������ ������
};

/// <summary>
/// ���̸� ���� �ʱ�ȭ��
/// </summary>
struct FTerrainBufInit_NoHeightMap
{
	_uint iNumVertexX;				// ���� ���� ����
	_uint iNumVertexZ;
	_uint iMaxWidth;				// ���� ������ ���� �ִ� ���� ��
};

/// <summary>
/// Height ���� ���Ǵ� 
/// </summary>
class CTerrainBufferComp final : public CVIBufferComp
{
	DERIVED_CLASS(CVIBufferComp, CTerrainBufferComp)
protected:
	explicit CTerrainBufferComp() = default;
	explicit CTerrainBufferComp(const CTerrainBufferComp& rhs);
	virtual ~CTerrainBufferComp() = default;

public:
	virtual HRESULT	Initialize_Prototype(void* Arg = nullptr) override;
	virtual HRESULT Initialize(void* Arg = nullptr) override;

public:
	static CTerrainBufferComp* Create();
	virtual CComponent* Clone(void* Arg = nullptr) override;

protected:
	virtual void	Free() override;

public:
	// �ʱ� ���� ���� �۾�
	HRESULT Create_Buffer(const FTerrainBufInit_HeightMap tInit);
	// �ʱ� ���� ���� �۾�, ����Ʈ ����
	HRESULT Create_Buffer(const FTerrainBufInit_NoHeightMap tInit);
	// �ܺο��� ���۰� �غ�Ǿ����� �˱� ���� �Լ�
	HRESULT IsRender_Ready();
	
public:
	const size_t Get_VertexCount() const { return m_viNumTerrainVertices.x * m_viNumTerrainVertices.z; }
	const _uint Get_VertexCountX() const { return m_viNumTerrainVertices.x; }
	const _uint Get_VertexCountZ() const { return m_viNumTerrainVertices.z; }

private:
	_int3	m_viNumTerrainVertices;		// �ͷ��� �� ����
	_float	m_fInterval;				// ���� ����
};

END