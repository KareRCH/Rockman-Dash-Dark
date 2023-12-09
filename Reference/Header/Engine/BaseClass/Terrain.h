#pragma once

#include "GameObject.h"
#include "Component/TerrainModelComp.h"

BEGIN(Engine)

/// <summary>
/// 지형 정보를 바탕으로 지형에 대한 기능을 가지도록 설계된 오브젝트
/// 바리에이션 없이 이 객체 하나가 모든 기능을 다 가지도록 설계한다.
/// </summary>
class ENGINE_DLL CTerrain : public CGameObject
{
	DERIVED_CLASS(CGameObject, CTerrain)

public:
	struct FInitTerrain : public FInitGameObjectBase
	{
		// 구조체기는 하지만 using에 등록 하기만 하는거라 상관 없음
		DERIVED_CLASS(FInitGameObjectBase, FInitTerrain)

		wstring	strHeightMapPath;
		_uint	iNumVertexCountX;
		_uint	iNumVertexCountY;
		_uint	iWidthX, iWidthY;
		_uint	iHeight;
	};

protected:
	explicit CTerrain() = default;
	explicit CTerrain(const CTerrain& rhs);
	virtual ~CTerrain() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* Arg = nullptr) override;
	virtual void	Priority_Tick(const _float& fTimeDelta) override;
	virtual void	Tick(const _float& fTimeDelta) override;
	virtual void	Late_Tick(const _float& fTimeDelta) override;
	virtual HRESULT	Render() override;

public:
	static CTerrain* Create();
	virtual CGameObject* Clone(void* Arg = nullptr) override;

protected:
	virtual void	Free() override;

private:
	HRESULT	Initialize_Component();
	HRESULT	Initialize_Component(const FInitTerrain& tInit);


protected:
	CTerrainModelComp* m_pTerrainModelComp = { nullptr };
};

END