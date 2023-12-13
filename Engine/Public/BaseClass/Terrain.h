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

		wstring	strHeightMapPath;	// 헤이트맵 정보가 담긴 파일 위치
		_uint	iNumVertexCountX;	// 버텍스 X 개수
		_uint	iNumVertexCountZ;	// 버텍스 Z 개수
		_uint	iMaxWidth;			// 너비, 정점의 개수가 많은 것 기준으로 간격이 조정된다.
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


public:
	HRESULT Create_Terrain(const FInitTerrain& tInit);
	// 터레인 생성시 헤이트 맵으로 부터 로드함.
	HRESULT Create_TerrainByHeightMap(const FInitTerrain& tInit);
	


protected:
	CTerrainModelComp* m_pTerrainModelComp = { nullptr };
};

END