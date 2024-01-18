#pragma once

#include "BaseClass/GameObject.h"
#include "Component/TerrainModelComp.h"

#include "Utility/ClassID.h"

BEGIN(Engine)

/// <summary>
/// ���� ������ �������� ������ ���� ����� �������� ����� ������Ʈ
/// �ٸ����̼� ���� �� ��ü �ϳ��� ��� ����� �� �������� �����Ѵ�.
/// </summary>
class ENGINE_DLL CTerrain : public CGameObject
{
	DERIVED_CLASS(CGameObject, CTerrain)

public:
	static const _uint g_ClassID = ECast(EObjectID::Terrain);

public:
	struct FInitTerrain : public FInitGameObjectBase
	{
		// ����ü��� ������ using�� ��� �ϱ⸸ �ϴ°Ŷ� ��� ����
		DERIVED_CLASS(FInitGameObjectBase, FInitTerrain)

		wstring	strHeightMapPath;		// ����Ʈ�� ������ ��� ���� ��ġ
		_uint	iNumVertexCountX;		// ���ؽ� X ����
		_uint	iNumVertexCountZ;		// ���ؽ� Z ����
		_uint	iMaxWidth;				// �ʺ�, ������ ������ ���� �� �������� ������ �����ȴ�.
		const D3D_SHADER_MACRO* pShaderMacro;	// ���̴� ��ũ�� ����, Tooló�� Ư���� ��쿡 ��ó���� �־��ֱ� ���ؼ� ���δ�.
	};

protected:
	explicit CTerrain();
	explicit CTerrain(const CTerrain& rhs);
	virtual ~CTerrain() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize_Prototype(const FInitTerrain& tInit);
	virtual HRESULT Initialize_Prototype(FSerialData& InputData);
	virtual HRESULT Initialize(void* Arg = nullptr) override;
	virtual HRESULT Initialize(FSerialData& InputData);
	virtual void	Priority_Tick(const _float& fTimeDelta) override;
	virtual void	Tick(const _float& fTimeDelta) override;
	virtual void	Late_Tick(const _float& fTimeDelta) override;
	virtual HRESULT	Render() override;

public:
	static CTerrain* Create();
	static CTerrain* Create(const FInitTerrain& tInit);
	static CTerrain* Create(FSerialData& InputData);
	virtual CGameObject* Clone(void* Arg = nullptr) override;
	virtual CGameObject* Clone(FSerialData& InputData);

protected:
	virtual void	Free() override;

public:
	virtual FSerialData SerializeData_Prototype() override;
	virtual FSerialData SerializeData() override;

private:
	HRESULT	Initialize_Component();
	HRESULT	Initialize_Component(const FInitTerrain& tInit);
	HRESULT	Initialize_Component(FSerialData& InputData);



public:
	HRESULT Create_Terrain(const FInitTerrain& tInit);
	// �ͷ��� ������ ����Ʈ ������ ���� �ε���.
	HRESULT Create_TerrainByHeightMap(const FInitTerrain& tInit);
	
protected:
	CTerrainModelComp* m_pTerrainModelComp = { nullptr };
};

template <>
struct TObjectTrait<CTerrain::g_ClassID>
{
	using Class = CTerrain;
};

END