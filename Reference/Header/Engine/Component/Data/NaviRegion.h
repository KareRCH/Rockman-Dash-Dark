#pragma once


#include "Base.h"
#include "DirectXCollision.h"
#include "Component/NavigationComponent.h"

BEGIN(Engine)


/// <summary>
/// �׺���̼��� ���� ������ ���ϴ� ��ü
/// ������ �������� �ִٸ� ������ ����ü�� Ư¡�� �����ٴ� ���̴�.
/// </summary>
class ENGINE_DLL CNaviRegion : public CBase
{
	DERIVED_CLASS(CBase, CNaviRegion)

public:
	enum DIRECTION { LEFT, TOP, FRONT, RIGHT, BOTTOM, BACK, DIR_END };

protected:
	explicit CNaviRegion();
	explicit CNaviRegion(const CNaviRegion& rhs);
	virtual ~CNaviRegion() = default;

public:
	virtual HRESULT Initialize(const BoundingBox& AABB, _uint& iIndex);


#ifdef _DEBUG
	HRESULT Render(class CEffectComponent* pEffectComp);
#endif // _DEBUG

public:
	static CNaviRegion* Create(const BoundingBox AABB, _uint iIndex);

protected:
	virtual void		Free() override;


public:
	_bool	IsIn(_fvector vPosition, _int* pNeighborIndex);
	void	SetUp_Neighbor(DIRECTION eDir, CNaviRegion* pNeighborRegion) { m_iNeighbors[eDir] = pNeighborRegion->m_iIndex; }

private:
	CNavigationComponent*	m_pNaviComp = { nullptr };
	BoundingBox				m_AABB;		// ����
	_int					m_iNeighbors[DIR_END] = { -1, -1, -1, -1, -1, -1 };	// �̿� ����
	_uint					m_iIndex = { 0 };									// �ڽ��� ID

};


END