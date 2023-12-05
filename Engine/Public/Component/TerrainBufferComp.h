#pragma once

#include "VIBufferComp.h"

BEGIN(Engine)

struct FTerrainBufInit
{
	wstring strHeightMapFilePath;	// ����Ʈ ���� ���
	_int	iWidth;					// �ִ� �ʺ����� ����, �ݵ�� ���簢������ ������
	_int	iHeight;				// �ִ� ������, �� ���� ���� �ִ� �����̰� ��������.
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
	// ��ġ�� ���� ���ε�
	HRESULT Bind_Buffer();
	// ���� �ε��̵� ���۸� �����
	HRESULT Render_Buffer();

private:
	_int3	m_viNumTerrainVertices;
};

END