#pragma once

#include "VIBufferComp.h"

BEGIN(Engine)

struct FTerrainBufInit
{
	wstring strHeightMapFilePath;	// ����Ʈ ���� ���
	_uint	iWidth;					// �ִ� �ʺ����� ����, �ݵ�� ���簢������ ������
	_uint	iHeight;				// �ִ� ������, �� ���� ���� �ִ� �����̰� ��������.
};

// ����Ʈ ���� �ʱ�ȭ��
struct FTerrainBufInit_NoHeight
{
	_uint iNumVertexX;
	_uint iNumVertexZ;
	_uint iWidth;
	_uint iHeight;
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
	HRESULT Create_Buffer(const FTerrainBufInit tInit);
	// �ʱ� ���� ���� �۾�, ����Ʈ ����
	HRESULT Create_Buffer(const FTerrainBufInit_NoHeight tInit);
	// ��ġ�� ���� ���ε�
	HRESULT Bind_Buffer();
	// ���� �ε��̵� ���۸� �����
	HRESULT Render_Buffer();
	// �ܺο��� ���۰� �غ�Ǿ����� �˱� ���� �Լ�
	HRESULT IsRender_Ready();

private:
	_int3	m_viNumTerrainVertices;		// �ͷ��� �� ����
	_float	m_fIntervalX;				// ���� ���� X
	_float	m_fIntervalZ;				// ���� ���� Z
};

END