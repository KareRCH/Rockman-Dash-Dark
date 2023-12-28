#pragma once

#include "Base.h"
#include "Component/D3D11DeviceComp.h"

BEGIN(Engine)

/// <summary>
/// 네비게이션의 각 삼각형에 대한 정보를 저장하는 클래스
/// </summary>
class ENGINE_DLL CNaviCell : public CBase
{
	DERIVED_CLASS(CBase, CNaviCell)

public:
	enum POINT { POINT_A, POINT_B, POINT_C, POINT_END };
	enum LINE { LINE_AB, LINE_BC, LINE_CA, LINE_END };

protected:
	explicit CNaviCell();
	explicit CNaviCell(const CNaviCell& rhs);
	virtual ~CNaviCell() = default;

public:
	virtual HRESULT Initialize(const _float3* pPoints, _uint iIndex);


#ifdef _DEBUG
	HRESULT Render(class CEffectComponent* pEffectComp, class CPipelineComp* pPipelineComp);
#endif // _DEBUG

public:
	static CNaviCell* Create(const _float3* pPoints, _uint iIndex);

protected:
	virtual void		Free() override;



#ifdef _DEBUG
private:
	class CNaviCellBufferComp* m_pVIBuffer = { nullptr };
#endif // _DEBUG



	
public:
	_bool	Compare_Points(const _float3* pSourPoint, const _float3* pDestPoint);
	_bool	IsIn(_fvector vPosition, _fmatrix WorldMatrix, _int* pNeighborIndex);
	void	Update(_fmatrix WorldMatrix);
	void	SetUp_Neighbor(LINE eLine, CNaviCell* pNeighborCell) 
	{
		m_iNeighbors[eLine] = pNeighborCell->m_iIndex;
	}

public:
	const _float3* Get_Point(POINT ePoint) const
	{
		return &m_vPoints[ePoint];
	}

private:
	_float3		m_vPoints[POINT_END] = {};
	_int		m_iNeighbors[LINE_END] = { -1, -1, -1 };
	_float3		m_vNormals[LINE_END] = {};
	_uint		m_iIndex = { 0 };



};

END