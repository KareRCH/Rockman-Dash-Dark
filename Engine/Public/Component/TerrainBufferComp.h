#pragma once

#include "VIBufferComp.h"

BEGIN(Engine)

struct FTerrainBufInit
{
	wstring strHeightMapFilePath;	// 헤이트 맵의 경로
	_int	iWidth;					// 최대 너비이자 면적, 반드시 정사각형으로 설정됨
	_int	iHeight;				// 최대 높낮이, 이 값을 통해 최대 높낮이가 정해진다.
};

/// <summary>
/// Height 맵이 사용되는 
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
	// 초기 버퍼 생성 작업
	HRESULT Create_Buffer(const FTerrainBufInit tInit);
	// 장치에 버퍼 바인드
	HRESULT Bind_Buffer();
	// 실제 인덱싱된 버퍼를 드로잉
	HRESULT Render_Buffer();

private:
	_int3	m_viNumTerrainVertices;
};

END