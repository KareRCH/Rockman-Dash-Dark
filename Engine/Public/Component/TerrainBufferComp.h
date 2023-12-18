#pragma once

#include "VIBufferComp.h"

BEGIN(Engine)


/// <summary>
/// 높이맵 있는 초기화자
/// </summary>
struct FTerrainBufInit_HeightMap
{
	wstring strHeightMapFilePath;	// 헤이트 맵의 경로
	_uint	iMaxWidth;				// 최대 너비이자 면적, 반드시 정사각형으로 설정됨
};

/// <summary>
/// 높이맵 없는 초기화자
/// </summary>
struct FTerrainBufInit_NoHeightMap
{
	_uint iNumVertexX;				// 지정 정점 개수
	_uint iNumVertexZ;
	_uint iMaxWidth;				// 많은 정점에 대한 최대 길이 값
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
	HRESULT Create_Buffer(const FTerrainBufInit_HeightMap tInit);
	// 초기 버퍼 생성 작업, 헤이트 없이
	HRESULT Create_Buffer(const FTerrainBufInit_NoHeightMap tInit);
	// 외부에서 버퍼가 준비되었는지 알기 위한 함수
	HRESULT IsRender_Ready();
	
public:
	const size_t Get_VertexCount() const { return m_viNumTerrainVertices.x * m_viNumTerrainVertices.z; }
	const _uint Get_VertexCountX() const { return m_viNumTerrainVertices.x; }
	const _uint Get_VertexCountZ() const { return m_viNumTerrainVertices.z; }

private:
	_int3	m_viNumTerrainVertices;		// 터레인 점 개수
	_float	m_fInterval;				// 점간 간격
};

END