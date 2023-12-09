#pragma once

#include "GameObject.h"
#include "Component/TerrainModelComp.h"

BEGIN(Engine)

/// <summary>
/// ���� ������ �������� ������ ���� ����� �������� ����� ������Ʈ
/// �ٸ����̼� ���� �� ��ü �ϳ��� ��� ����� �� �������� �����Ѵ�.
/// </summary>
class ENGINE_DLL CTerrain : public CGameObject
{
	DERIVED_CLASS(CGameObject, CTerrain)

public:
	struct FInitTerrain : public FInitGameObjectBase
	{
		// ����ü��� ������ using�� ��� �ϱ⸸ �ϴ°Ŷ� ��� ����
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